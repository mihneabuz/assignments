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

// calculate horizontal sum of elems in x
float my_m256_sum(__m256 x) {
    __m128 hi = _mm256_extractf128_ps(x, 1);
    __m128 lo = _mm256_extractf128_ps(x, 0);
    lo = _mm_add_ps(hi, lo);
    hi = _mm_movehl_ps(hi, lo);
    lo = _mm_add_ps(hi, lo);
    hi = _mm_shuffle_ps(lo, lo, 1);
    lo = _mm_add_ss(hi, lo);
    return _mm_cvtss_f32(lo);
}

// calculate horizontal sum of elems in x
float my_m128_sum(__m128 x) {
    __m128 result;
    result = _mm_hadd_ps (x, x);
    result = _mm_hadd_ps (result, result);
    return _mm_cvtss_f32(result);
}

void f_vector_op_avx(float *A, float *B, float *C, float *D, int n)
{
    /* TODO: optimise the code above, using AVX */
    float sum = 0;

    // Calculating the sum
    int processed = 0;
    while (processed <= n - 8) {
        __m256 Ax = _mm256_loadu_ps(A + processed);
        __m256 Bx = _mm256_loadu_ps(B + processed);
        sum += my_m256_sum(_mm256_mul_ps(Ax, Bx));
        processed += 8;
    }
    while (processed <= n - 4) {
        // sum in batch of 4
        __m128 Ax = _mm_loadu_ps(A + processed);
        __m128 Bx = _mm_loadu_ps(B + processed);
        sum += my_m128_sum(_mm_mul_ps(Ax, Bx));
        processed += 4;
    }
    while (processed < n) {
        // remaining elems
        sum += A[processed] * B[processed];
        processed += 1;
    }

    // Making sum vectors
    __m256 sum_vec256 = _mm256_set1_ps(sum);
    __m128 sum_vec128 = _mm_set1_ps(sum);
    
    // Calculating the square root + sum_vectors
    // maybe should use rsqrt(x) * x ? could't tell if it was faster
    processed = 0;
    while (processed <= n - 8) {
        // sqrt in batches of 8
        __m256 Cx = _mm256_loadu_ps(C + processed);
        Cx = _mm256_add_ps(_mm256_sqrt_ps(Cx), sum_vec256);
        _mm256_storeu_ps(D + processed, Cx);
        processed += 8;
    }
    if (processed <= n - 4) {
        // sqrt in batch of 4
        __m128 Cx = _mm_loadu_ps(C + processed);
        Cx = _mm_add_ps(_mm_sqrt_ps(Cx), sum_vec128);
        _mm_storeu_ps(D + processed, Cx);
        processed += 4;
    }
    while (processed < n) {
        // remaining elems
        D[processed] = sqrt(C[processed]) + sum;
        processed += 1;
    }
}
