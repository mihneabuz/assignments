/* I included the AVX header for you */
#include <immintrin.h>
#include <math.h>

void f_vector_op(float *A, float *B, float *C, float *D, int n)
{
    float sum = 0;
    /* A * B; result stored in sum */
    for (int i = 0; i < n; i++)
        sum += A[i] * B[i];

    /* Computing D */
    for (int i = 0; i < n; i++)
        D[i] = sqrt(C[i]) + sum;
}

__m256 my_mm256_hadd_all(__m256 v)
{
    // sumeaza orizontal toate elementele din v
    register __m256 x = _mm256_permute2f128_ps(v, v, 1);
    register __m256 y = _mm256_add_ps(v, x);
    x = _mm256_shuffle_ps(y, y, _MM_SHUFFLE(2, 3, 0, 1));
    x = _mm256_add_ps(x, y);
    y = _mm256_shuffle_ps(x, x, _MM_SHUFFLE(1, 0, 3, 2));
    return _mm256_add_ps(x, y);
}

void f_vector_op_avx(float *A, float *B, float *C, float *D, int n)
{
    // sum = [0] * 8
    register __m256 sum = _mm256_set1_ps(0.0);

    int i;
    float *A_ptr = A, *B_ptr = B;
    for (i = 0; i <= n - 8; i += 8, A_ptr += 8, B_ptr += 8)
        // sum = (sum + (A_ptr * B_ptr))
        sum = _mm256_add_ps(sum, _mm256_mul_ps(_mm256_loadu_ps(A_ptr),
                                               _mm256_loadu_ps(B_ptr)));
   
    // sum = hadd_all(sum)
    sum = my_mm256_hadd_all(sum);

    register float *C_ptr = C, *D_ptr = D;
    for (i = 0; i <= n - 8; i += 8, C_ptr += 8, D_ptr += 8)
        // D_ptr = sqrt(C_ptr) + sum
        _mm256_storeu_ps(D_ptr, _mm256_add_ps(_mm256_sqrt_ps(
                        _mm256_loadu_ps(C_ptr)), sum));
}
