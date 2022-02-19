#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_COORDONATORS  3
#define MAX_WORKERS      16

#define ROOT 0

enum {
    COORDONATOR,
    WORKER
};

void send_mpi(int* values, int count, int src, int dest) {
    printf("M(%d,%d)\n", src, dest);
    MPI_Send(values, count, MPI_INT, dest, 1, MPI_COMM_WORLD);
}

void recv_mpi(int* values, int count, int from) {
    MPI_Status status;
    MPI_Recv(values, count, MPI_INT, from, 1, MPI_COMM_WORLD, &status);
}

char* verbose_topology(int rank, int *topology_counts, int topology[NUM_COORDONATORS][MAX_WORKERS]) {
    char* topology_str = malloc(128);
    sprintf(topology_str, "%d -> ", rank);

    for (int i = 0; i < NUM_COORDONATORS; i++) {
        char r = '0' + i;
        strncat(topology_str, &r, 1);
        strncat(topology_str, ":", 1);

        for (int j = 0; j < topology_counts[i]; j++) {
            r = '0' + topology[i][j];
            strncat(topology_str, &r, 1);
            if (j < topology_counts[i] - 1)
                strncat(topology_str, ",", 1);
        }

        strncat(topology_str, " ", 1);
    }

    return topology_str;
}

int main (int argc, char *argv[])
{
    int numtasks, rank, role, N;

    N = atoi(argv[1]);

    int other_coordonators[NUM_COORDONATORS - 1];
    int topology_counts[NUM_COORDONATORS];
    int topology[NUM_COORDONATORS][MAX_WORKERS];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank < NUM_COORDONATORS) {
        role = COORDONATOR;
        if (rank == 0) {
            other_coordonators[0] = 1;
            other_coordonators[1] = 2;
        } else if (rank == 1) {
            other_coordonators[0] = 0;
            other_coordonators[1] = 2;
        } else if (rank == 2) {
            other_coordonators[0] = 0;
            other_coordonators[1] = 1;
        }
    } else { 
        role = WORKER;
    }

    if (role == COORDONATOR) {
        int num_workers;
        int workers[MAX_WORKERS];

        char* workers_file_name = malloc(15);
        strcpy(workers_file_name, "cluster_.txt");
        workers_file_name[7] = '0' + rank;

        FILE *workers_file = fopen(workers_file_name, "rt");
        char line[64];
        fgets(line, sizeof line, workers_file);
        num_workers = atoi(line);

        int k = 0;
        while(fgets(line, sizeof line, workers_file) != NULL)
            workers[k++] = atoi(line);

        topology_counts[rank] = num_workers;
        for (int i = 0; i < num_workers; i++)
            topology[rank][i] = workers[i];

        // send number of workers
        send_mpi(&num_workers, 1, rank, other_coordonators[0]);
        send_mpi(&num_workers, 1, rank, other_coordonators[1]);

        // receive number of workers
        recv_mpi(&topology_counts[other_coordonators[0]], 1, other_coordonators[0]);
        recv_mpi(&topology_counts[other_coordonators[1]], 1, other_coordonators[1]);

        // send workers
        send_mpi(workers, num_workers, rank, other_coordonators[0]);
        send_mpi(workers, num_workers, rank, other_coordonators[1]);

        // receive workers
        recv_mpi(topology[other_coordonators[0]], topology_counts[other_coordonators[0]], other_coordonators[0]);
        recv_mpi(topology[other_coordonators[1]], topology_counts[other_coordonators[1]], other_coordonators[1]);

        printf("%s\n", verbose_topology(rank, topology_counts, topology));

        // send topology to connected workers
        for (int i = 0; i < num_workers; i++) {
            send_mpi(&rank, 1, rank, workers[i]);

            for (int j = 0; j < NUM_COORDONATORS; j++) {
                send_mpi(&topology_counts[j], 1, rank, workers[i]);
                send_mpi(topology[j], topology_counts[j], rank, workers[i]);
            }
        }

        // create and distribute data between coordinators
        int total_workers = topology_counts[0] + topology_counts[1] + topology_counts[2]; 
        int step = N / total_workers;
        int* V;
        int count;
        if (rank == ROOT) {
            V = malloc(N * sizeof(int));

            for (int i = 0; i < N; i++)
                V[i] = i;

            count = num_workers * step;

            int temp = topology_counts[other_coordonators[0]] * step;
            send_mpi(&temp, 1, rank, other_coordonators[0]);
            send_mpi(V + step * num_workers,
                     topology_counts[other_coordonators[0]] * step,
                     rank, other_coordonators[0]);

            if (step * total_workers < N) {
                int final_count = N - step * (num_workers + topology_counts[other_coordonators[0]]);
                send_mpi(&final_count, 1, rank, other_coordonators[1]);
                send_mpi(V + step * (num_workers + topology_counts[other_coordonators[0]]),
                         final_count, rank, other_coordonators[1]);
            } else {
                int temp = topology_counts[other_coordonators[1]] * step;
                send_mpi(&temp, 1, rank, other_coordonators[1]);
                send_mpi(V + step * (num_workers + topology_counts[other_coordonators[0]]), 
                         temp, rank, other_coordonators[1]);
            }

        } else {
            recv_mpi(&count, 1, ROOT);

            V = malloc(count * sizeof(int));
            recv_mpi(V, count, ROOT);
        }

        // distribute task to workers
        for (int i = 0; i < num_workers; i++) {
            int send_count;
            if (i == num_workers - 1)
                send_count = count - (i * step); 
            else
                send_count = step;

            send_mpi(&send_count, 1, rank, workers[i]);
            send_mpi(V + i * step, send_count, rank, workers[i]);
        }

        // receive computed data from workers
        for (int i = 0; i < num_workers; i++) {
            int send_count;
            if (i == num_workers - 1)
                send_count = count - (i * step); 
            else
                send_count = step;

            recv_mpi(V + i * step, send_count, workers[i]);
        }

        // reassemble V in ROOT
        if (rank == ROOT) {
            recv_mpi(V + step * num_workers, topology_counts[other_coordonators[0]] * step, other_coordonators[0]);

            if (step * total_workers < N) {
                int final_count = N - step * (num_workers + topology_counts[other_coordonators[0]]);
                recv_mpi(V + step * (num_workers + topology_counts[other_coordonators[0]]), 
                         final_count, other_coordonators[1]);
            } else {
                recv_mpi(V + step * (num_workers + topology_counts[other_coordonators[0]]), 
                         topology_counts[other_coordonators[1]] * step, other_coordonators[1]);
            }

            printf("Rezultat:");
            for (int i = 0; i < N; i++) {
                printf(" %d", V[i]);
            }

        } else {
            send_mpi(V, count, rank, ROOT);
        }


    } else if (role == WORKER) {
        int master;
        // get master rank
        recv_mpi(&master, 1, MPI_ANY_SOURCE);

        // get topology from master
        for (int j = 0; j < NUM_COORDONATORS; j++) {
            recv_mpi(&topology_counts[j], 1, master);
            recv_mpi(topology[j], topology_counts[j], master);
        }

        printf("%s\n", verbose_topology(rank, topology_counts, topology));

        // get data
        int count;
        recv_mpi(&count, 1, master);

        int *V = malloc(count * sizeof(int));
        recv_mpi(V, count, master);

        // compute
        for (int i = 0; i < count; i++)
            V[i] *= 2;

        // send computed data
        send_mpi(V, count, rank, master);
    }

    MPI_Finalize();
}

