#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define TEST_SIZE 1000

void f_vector_op(float *A, float *B, float *C, float *D, int n);
void f_vector_op_avx(float *A, float *B, float *C, float *D, int n);
void i_vector_op(int *A, int *B, int *C, int n);
void i_vector_op_avx(int *A, int *B, int *C, int n);

int main() {

    int n = TEST_SIZE;
    float A[TEST_SIZE];
    float B[TEST_SIZE];
    float C[TEST_SIZE];
    float D[TEST_SIZE];

    int i;
    srand(time(NULL));
    printf("Randomizing numbers...\n");
    for (i = 0; i < n; i++) {
        float seed = (float)(rand() % 1000);
        A[i] = seed / 100 * 1.2345;
        B[i] = seed / 100 * 2.14321;
        C[i] = seed / 100 * 0.7891;
    }

    printf("Here's some examples:\n");
    int ex = rand() % TEST_SIZE;
    f_vector_op(A, B, C, D, n);
    printf("A[%d] = %f B[%d] = %f C[%d] = %f D[%d] = %f \n", 
            ex, A[ex], ex, B[ex], ex, C[ex], ex, D[ex]);
    f_vector_op_avx(A, B, C, D, n);
    printf("A[%d] = %f B[%d] = %f C[%d] = %f D[%d] = %f \n", 
            ex, A[ex], ex, B[ex], ex, C[ex], ex, D[ex]);

    int Ax[TEST_SIZE];
    int Bx[TEST_SIZE];
    int Cx[TEST_SIZE];
    int Cy[TEST_SIZE];
    for (i = 0; i < n; i++) {
        int seed = rand() % 10000;
        Ax[i] = seed / 123;
        Bx[i] = seed / 37;
    }
    i_vector_op(Ax, Bx, Cx, TEST_SIZE);
    i_vector_op_avx(Ax, Bx, Cy, TEST_SIZE);
    i = 90;
    for (i ; i <= 100; i++) {
        printf("%d) A = %d B = %d   C1 = %d  C2 = %d\n", i, Ax[i], Bx[i], Cx[i], Cy[i]);
    }
}










