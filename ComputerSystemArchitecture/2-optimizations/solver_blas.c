/*
 * Tema 2 ASC
 * 2022 Spring
 */
#include "utils.h"
#include "cblas.h"
#include "string.h"

/*
 * Add your BLAS implementation here
 */
double* my_solver(int N, double *A, double *B) {
    double* result = calloc(N * N, sizeof(double));
    double* temp = calloc(N * N, sizeof(double));

    memcpy(result, B, N * N * sizeof(double));
    cblas_dtrmm(CblasRowMajor, CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, N, N, 1.f, A, N, result, N);
    cblas_dtrmm(CblasRowMajor, CblasRight, CblasUpper, CblasTrans, CblasNonUnit, N, N, 1.f, A, N, result, N);

    cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, N, N, N, 1.f, B, N, B, N, 0.f, temp, N);

    for (int i = 0; i < N * N; i++)
        result[i] += temp[i];    

    free(temp);
    return result;
}
