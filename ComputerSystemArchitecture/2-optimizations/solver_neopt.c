/*
 * Tema 2 ASC
 * 2022 Spring
 */
#include "utils.h"

void matmul(int N, double *A, double *B, double *C) {
    // multiplies a regular matrix with a regular matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

void matmul_2upper(int N, double *A, double *B, double *C) {
    // multiplies a regular matrix with an upper triangle matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int k = 0; k < j + 1; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

void matmul_2lower(int N, double *A, double *B, double *C) {
    // multiplies a regular matrix with a lower triangle matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int k = j; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

void matadd_(int N, double *A, double *B) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] += B[i * N + j];
        }
    }
}

void mattrans(int N, double *A, double *C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i * N + j] = A[j * N + i];
        }
    }
}

void mattrans_upper(int N, double *A, double *C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < i + 1; j++) {
            C[i * N + j] = A[j * N + i];
        }
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
		free(temp1);
		free(result);
		return NULL;	
    }

    // result = B x A x A^t where A upper triangle, A^t lower triangle
    matmul_2upper(N, B, A, temp1);
    mattrans_upper(N, A, temp2);
    matmul_2lower(N, temp1, temp2, result);

    // temp2 = B^t x B
    mattrans(N, B, temp1);
    matmul(N, temp1, B, temp2);

    // result = result + temp2
    matadd_(N, result, temp2);

    free(temp1);
    free(temp2);
    return result;
}
