/*
 * Tema 2 ASC
 * 2022 Spring
 */
#include "utils.h"

void matmul(const register int N, register double *A, register double *B, register double *C) {
    // multiplies a regular matrix with a regular matrix -- unused
    const register int N_squared = N * N;
    for (register int k = 0; k < N; k++) {
        for (register int i = 0; i < N; i++) {
            for (register int j = 0; j < N; j++) {
                *C += *A * *B;
                C++;
                B++;
            }
            B -= N;
            A += N;
        }
        C -= N_squared;
        A -= N_squared;

        B += N;
        A ++;
    }
}

void matmul_trans_normal(const register int N, register double *A, register double *C) {
    // C += A^t * A
    const register int N_squared = N * N;
    for (register int k = 0; k < N; k++) {
        for (register int i = 0; i < N; i++) {
            for (register int j = 0; j < N; j++) {
                *C += A[i] * A[j];
                C++;
            }
        }
        C -= N_squared;
        A += N;
    }
}

void matmul_2upper(const register int N, register double *A, register double *B, register double *C) {
    // multiplies a regular matrix with an upper triangle matrix
    for (register int i = 0; i < N; i++) {
        for (register int j = 0; j < N; j++) {
            register double sum = 0;
            const int h = j + 1;

            for (register int k = 0; k < h; k++) {
                sum += *A * *B;
                A++;
                B += N;
            }
            A -= h;
            B -= (N * h) - 1;

            *C = sum;
            C++;
        }

        A += N;
        B -= N;
    }
}

void matmul_2lower(const register int N, register double *A, register double *B, register double *C) {
    // multiplies a regular matrix with a lower triangle matrix
    const int N_squared = N * N;
    for (register int i = 0; i < N; i++) {
        for (register int j = 0; j < N; j++) {
            register double sum = 0;

            A += j;
            B += N * j;
            for (register int k = j; k < N; k++) {
                sum += *A * *B;
                A++;
                B += N;
            }
            A -= N;
            B -= N_squared - 1;

            *C = sum;
            C++;
        }

        A += N;
        B -= N;
    }
}

void mattrans_upper(const register int N, register double *A, register double *C) {
    for (register int i = 0; i < N; i++) {
        const register int h = i + 1;
        for (register int j = 0; j < h; j++) {
            *C = *A;
            C++;
            A += N;
        }
        A -= N * h - 1;
        C += N - h;
    }
}

double* my_solver(int N, double *A, double *B) {
    double* result = calloc(N * N, sizeof(double));
    if (!result) {
	    return NULL;
    }
    double* temp1 = calloc(N * N, sizeof(double));
    if (!temp1) {
	    free(result);
	    return NULL;
    }
    double* temp2 = calloc(N * N, sizeof(double));
    if (!temp2) {
	    free(result);
	    free(temp1);
	    return NULL;
    }

    matmul_2upper(N, B, A, temp1);
    mattrans_upper(N, A, temp2);
    matmul_2lower(N, temp1, temp2, result);
    matmul_trans_normal(N, B, result);

    free(temp1);
    free(temp2);
    return result;
}
