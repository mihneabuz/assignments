#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "genetic_algorithm.h"

#include <pthread.h>
#include "pthread_barrier_mac.h"

int read_input(sack_object **objects, int *object_count, int *sack_capacity, int *generations_count, int *processor_count, int argc, char *argv[])
{
	FILE *fp;

	if (argc < 4) {
		fprintf(stderr, "Usage:\n\t./tema1_par [in_file] [generations_count] [processor_count]\n");
		return 0;
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		return 0;
	}

	if (fscanf(fp, "%d %d", object_count, sack_capacity) < 2) {
		fclose(fp);
		return 0;
	}

	if (*object_count % 10) {
		fclose(fp);
		return 0;
	}

	sack_object *tmp_objects = (sack_object *) calloc(*object_count, sizeof(sack_object));

	for (int i = 0; i < *object_count; ++i) {
		if (fscanf(fp, "%d %d", &tmp_objects[i].profit, &tmp_objects[i].weight) < 2) {
			free(objects);
			fclose(fp);
			return 0;
		}
	}

	fclose(fp);

	*generations_count = (int) strtol(argv[2], NULL, 10);
	if (*generations_count == 0) {
		free(tmp_objects);

		return 0;
	}

    *processor_count = (int) strtol(argv[3], NULL, 10);
	if (*processor_count == 0) {
		free(tmp_objects);

		return 0;
	}

    if (*processor_count > 16) {
        *processor_count = 16;
    }


	*objects = tmp_objects;

	return 1;
}

void print_objects(const sack_object *objects, int object_count)
{
	for (int i = 0; i < object_count; ++i) {
		printf("%d %d\n", objects[i].weight, objects[i].profit);
	}
}

void print_generation(const individual *generation, int limit)
{
	for (int i = 0; i < limit; ++i) {
        int chr = 0;
		for (int j = 0; j < generation[i].chromosome_length; ++j) {
            if (generation[i].chromosomes[j])
                chr = chr | (1 << j);
		}
		printf("%d - index %d - chrom %d - fitness %d\n", i, generation[i].index, chr, generation[i].fitness);
	}
}

void print_best_fitness(const individual *generation)
{
	printf("%d\n", generation[0].fitness);
}

void compute_fitness_function(const sack_object *objects, individual *generation, int start, int end, int sack_capacity)
{
	int weight;
	int profit;

	for (int i = start; i < end; ++i) {
		weight = 0;
		profit = 0;

		for (int j = 0; j < generation[i].chromosome_length; ++j) {
			if (generation[i].chromosomes[j]) {
				weight += objects[j].weight;
				profit += objects[j].profit;
			}
		}

		generation[i].fitness = (weight <= sack_capacity) ? profit : 0;
	}
}

int cmpfunc(const void *a, const void *b)
{
    return ((individual*)b)->fitness - ((individual*)a)->fitness;
	// int i;
	// individual *first = (individual *) a;
	// individual *second = (individual *) b;

	// int res = second->fitness - first->fitness; // decreasing by fitness
	// if (res == 0) {
	// 	int first_count = 0, second_count = 0;

 //        for (i = 0; i < first->chromosome_length && i < second->chromosome_length; ++i) {
 //            first_count += first->chromosomes[i];
 //            second_count += second->chromosomes[i];
 //        }

 //        res = first_count - second_count; // increasing by number of objects in the sack
 //        if (res == 0) {
 //            return second->index - first->index;
 //        }
 //    }

	// return res;
}

void mutate_bit_string_1(const individual *ind, int generation_index)
{
	int i, mutation_size;
	int step = 1 + generation_index % (ind->chromosome_length - 2);

	if (ind->index % 2 == 0) {
		// for even-indexed individuals, mutate the first 40% chromosomes by a given step
		mutation_size = ind->chromosome_length * 4 / 10;
		for (i = 0; i < mutation_size; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	} else {
		// for even-indexed individuals, mutate the last 80% chromosomes by a given step
		mutation_size = ind->chromosome_length * 8 / 10;
		for (i = ind->chromosome_length - mutation_size; i < ind->chromosome_length; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	}
}

void mutate_bit_string_2(const individual *ind, int generation_index)
{
	int step = 1 + generation_index % (ind->chromosome_length - 2);

	// mutate all chromosomes by a given step
	for (int i = 0; i < ind->chromosome_length; i += step) {
		ind->chromosomes[i] = 1 - ind->chromosomes[i];
	}
}

void crossover(individual *parent1, individual *child1, int generation_index)
{
	individual *parent2 = parent1 + 1;
	individual *child2 = child1 + 1;
	int count = 1 + generation_index % parent1->chromosome_length;

	memcpy(child1->chromosomes, parent1->chromosomes, count * sizeof(int));
	memcpy(child1->chromosomes + count, parent2->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));

	memcpy(child2->chromosomes, parent2->chromosomes, count * sizeof(int));
	memcpy(child2->chromosomes + count, parent1->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));
}

void copy_individual(const individual *from, const individual *to)
{
	memcpy(to->chromosomes, from->chromosomes, from->chromosome_length * sizeof(int));
}

void free_generation(individual *generation)
{
	int i;
	for (i = 0; i < generation->chromosome_length; ++i) {
		free(generation[i].chromosomes);
		generation[i].chromosomes = NULL;
		generation[i].fitness = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_THREADS 16

typedef struct Merge_Controller {
    int segments;
    int *split_points;
} merge_controller;

/*
    10 N -> 3 P

      0      1       2
    # # #  # # #  # # # #

    segments = 3
    split_points = 0 3 6 10

         0          1
    # # # # # #  # # # #

    segments = 2
    split_points = 0 6 10

             0
    # # # # # # # # # #

    DONE

*/

typedef struct Thread_Info {
    char id;
    char processor_count;
    int start, end;
    int chromosome_length, generations, capacity;
    const sack_object *objects;
    individual *current_generation, *next_generation;
    char compute_flags;
    merge_controller *controller;
    pthread_barrier_t *barrier;
} thread_info;

enum {
    CONTROLLER      =  1,  // manages merge after sorting
    ELITE_CHILDREN  =  2,  // copies elite children
    MUTATION_1      =  4,  // computes mutation 1
    MUTATION_2      =  8,  // computes mutation 2
    CROSSOVER       = 16,  // computes crossover
};

void merge(individual* array, int start, int mid, int end, individual* dest) {

    array = array + start;
    mid = mid - start;
    end = end - start;
    start = 0;

    int iA = start;
	int iB = mid;
	int i;

	for (i = start; i < end; i++) {
		if (end == iB || (iA < mid && cmpfunc(array + iA, array + iB) < 0)) {
			memcpy(dest + i, array + iA, sizeof(individual));
			iA++;
		} else {
			memcpy(dest + i, array + iB, sizeof(individual));
			iB++;
		}
	}
}

void *thread_func(void* data) {
    thread_info t_info = *(thread_info*)data;

    int chromosome_length = t_info.chromosome_length;
    individual* current_generation = t_info.current_generation;
    individual* next_generation = t_info.next_generation;
    individual* temp = (individual*) calloc(chromosome_length, sizeof(individual));
    merge_controller* controller = t_info.controller;

    for (int i = t_info.start; i < t_info.end; i++) {
        current_generation[i].fitness = 0;
		current_generation[i].chromosomes = (int*) calloc(chromosome_length, sizeof(int));
		current_generation[i].chromosomes[i] = 1;
		current_generation[i].index = i;
		current_generation[i].chromosome_length = chromosome_length;

		next_generation[i].fitness = 0;
		next_generation[i].chromosomes = (int*) calloc(chromosome_length, sizeof(int));
		next_generation[i].index = i;
		next_generation[i].chromosome_length = chromosome_length;
    }

    for (int gen = 0; gen < t_info.generations; gen++) {
        compute_fitness_function(t_info.objects, current_generation, t_info.start, t_info.end, t_info.capacity);
        qsort(current_generation + t_info.start, t_info.end - t_info.start, sizeof(individual), cmpfunc);

        if (t_info.compute_flags & CONTROLLER) {
            controller->segments = t_info.processor_count;
            controller->split_points[0] = 0;
        }

        controller->split_points[t_info.id + 1] = t_info.end;

        // merge sorted results
        pthread_barrier_wait(t_info.barrier);
        while (controller->segments > 1) {
            // merge segments
            if (t_info.id % 2 == 0 && t_info.id < controller->segments - (controller->segments % 2)) {
                int start = controller->split_points[(int)t_info.id];
                int mid = controller->split_points[(int)t_info.id + 1];
                int end = controller->split_points[(int)t_info.id + 2];
                merge(current_generation, start, mid, end, temp);
                memcpy(current_generation + start, temp, sizeof(individual) * (end - start));
            }

            // update segment information
            pthread_barrier_wait(t_info.barrier);
            if (t_info.compute_flags & CONTROLLER) {
                for (int i = 1; i < controller->segments - (controller->segments % 2); i++) {
                    for (int j = i + 1; j < controller->segments + 1; j++) {
                        controller->split_points[j - 1] = controller->split_points[j];
                    }
                }
                if (controller->segments % 2 == 0) {
                    controller->segments = controller->segments / 2;
                } else {
                    controller->segments = controller->segments / 2 + 1;
                }
            }
            pthread_barrier_wait(t_info.barrier);
        }
        pthread_barrier_wait(t_info.barrier);

        int cursor, count;

        count = chromosome_length * 3 / 10;
        if (t_info.compute_flags & ELITE_CHILDREN) {
            for (int i = 0; i < count; ++i) {
                copy_individual(current_generation + i, next_generation + i);
            }
        }
        cursor = count;

        count = chromosome_length * 2 / 10;
        if (t_info.compute_flags & MUTATION_1) {
            for (int i = 0; i < count; ++i) {
                copy_individual(current_generation + i, next_generation + cursor + i);
                mutate_bit_string_1(next_generation + cursor + i, gen);
            }
        }
        cursor += count;

        count = chromosome_length * 2 / 10;
        if (t_info.compute_flags & MUTATION_2) {
            for (int i = 0; i < count; ++i) {
                copy_individual(current_generation + i + count, next_generation + cursor + i);
                mutate_bit_string_2(next_generation + cursor + i, gen);
            }
        }
        cursor += count;

        count = chromosome_length * 3 / 10;
        if (t_info.compute_flags & CROSSOVER) {
            if (count % 2 == 1) {
                copy_individual(current_generation + chromosome_length - 1, next_generation + cursor + count - 1);
                count--;
            }

            for (int i = 0; i < count; i += 2) {
                crossover(current_generation + i, next_generation + cursor + i, gen);
            }
        }

        individual* tmp;
        tmp = current_generation;
        current_generation = next_generation;
        next_generation = tmp;

        pthread_barrier_wait(t_info.barrier);

        for (int i = t_info.start; i < t_info.end; ++i)
            current_generation[i].index = i;

		if (t_info.id == 0 && gen % 5 == 0)
			print_best_fitness(current_generation);
    }

    compute_fitness_function(t_info.objects, current_generation, t_info.start, t_info.end, t_info.capacity);
    free(temp);

  	pthread_exit(NULL);
}

void run_genetic_algorithm(const sack_object *objects, int object_count, int generations_count, int sack_capacity, int processor_count)
{
	individual *current_generation = (individual*) calloc(object_count, sizeof(individual));
	individual *next_generation = (individual*) calloc(object_count, sizeof(individual));

    pthread_t threads[MAX_THREADS];
    thread_info t_info[MAX_THREADS];
  	void *status;
    int result;

    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, processor_count);
    
    merge_controller merge_controller;
    merge_controller.segments = processor_count;
    merge_controller.split_points = (int*) calloc(processor_count + 1, sizeof(int));

    char t_flags[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++)
        t_flags[i] = 0;

    // assign tasks to different threads
    if (processor_count == 1) {
        t_flags[0] = CONTROLLER | ELITE_CHILDREN | MUTATION_1 | MUTATION_2 | CROSSOVER;
    } else if (processor_count == 2) {
        t_flags[0] = CONTROLLER | ELITE_CHILDREN | MUTATION_1;
        t_flags[1] = CROSSOVER | MUTATION_2;
    } else if (processor_count == 3) {
        t_flags[0] = CONTROLLER | MUTATION_1;
        t_flags[1] = ELITE_CHILDREN | MUTATION_2;
        t_flags[2] = CROSSOVER;
    } else if (processor_count >= 4) {
        t_flags[0] = CONTROLLER | ELITE_CHILDREN;
        t_flags[1] = CROSSOVER;
        t_flags[2] = MUTATION_1;
        t_flags[3] = MUTATION_2;
    }
     
    int step_size = object_count / processor_count;
    for (int i = 0; i < processor_count; i++) {
        t_info[i].id = i;
        t_info[i].processor_count = processor_count;

        t_info[i].start = step_size * i;
        t_info[i].end = i == processor_count - 1 ? object_count : step_size * (i + 1);

        t_info[i].chromosome_length = object_count;
        t_info[i].generations = generations_count;
        t_info[i].capacity = sack_capacity;

        t_info[i].objects = objects;
        t_info[i].current_generation = current_generation;
        t_info[i].next_generation = next_generation;

        t_info[i].compute_flags = t_flags[i];

        t_info[i].controller = &merge_controller;
        t_info[i].barrier = &barrier;

		result = pthread_create(&threads[i], NULL, thread_func, &t_info[i]);

		if (result) {
	  		printf("Eroare la crearea thread-ului %d\n", i);
	  		exit(-1);
		}
    }

    for (int i = 0; i < processor_count; i++) {
		result = pthread_join(threads[i], &status);

		if (result) {
	  		printf("Eroare la asteptarea thread-ului %d\n", i);
	  		exit(-1);
		}
    }

    int best_fitness = 0;
    for (int i = 0; i < object_count; i++) {
        if (current_generation[i].fitness > best_fitness)
            best_fitness = current_generation[i].fitness;
    }
	printf("%d\n", best_fitness);

	free_generation(current_generation);
	free_generation(next_generation);
	free(current_generation);
	free(next_generation);
    free(merge_controller.split_points);
}

////////////////////////////////////////////////////////////////////////////////////////
