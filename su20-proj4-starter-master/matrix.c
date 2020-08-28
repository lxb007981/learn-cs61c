#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/* Generates a random double between low and high */
double rand_double(double low, double high)
{
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high)
{
    srand(seed);
    /*
    for (int i = 0; i < result->rows; i++)
    {
        for (int j = 0; j < result->cols; j++)
        {
            set(result, i, j, rand_double(low, high));
        }
    }
    */
#pragma omp parallel for
    for (int i = 0; i < result->rows * result->cols; ++i)
    {
        double range = (high - low);
        double div = RAND_MAX / range;
        result->data[i] = low + (rand() / div);
    }
}
/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values, or if any
 * call to allocate memory in this function fails. Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols)
{
    /* TODO: YOUR CODE HERE */
    if (rows <= 0 || cols <= 0)
        return -1;
    matrix *matp = (matrix *)malloc(sizeof(matrix));
    if (matp == NULL)
        return -1;
    matp->rows = rows;
    matp->cols = cols;
    matp->ref_cnt = 1;
    matp->parent = NULL;
    double *data = (double *)malloc(sizeof(double) * rows * cols);
    if (data == NULL)
    {
        free(matp);
        return -1;
    }
    memset(data, 0, sizeof(double) * rows * cols);
    matp->data = data;
    *mat = matp;
    return 0;
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`.
 * You should return -1 if either `rows` or `cols` or both are non-positive or if any
 * call to allocate memory in this function fails. Return 0 upon success.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols)
{
    /* TODO: YOUR CODE HERE */
    if (rows <= 0 || cols <= 0 || from == NULL)
        return -1;
    matrix *matp = (matrix *)malloc(sizeof(matrix));
    if (matp == NULL)
        return -1;
    matp->rows = rows;
    matp->cols = cols;
    matp->ref_cnt = 1;
    matp->parent = from;
    matp->data = from->data + offset;
    from->ref_cnt += 1;
    *mat = matp;
    return 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice and has no existing slices,
 * or if `mat` is the last existing slice of its parent matrix and its parent matrix has no other references
 * (including itself). You cannot assume that mat is not NULL.
 */
void deallocate_matrix(matrix *mat)
{
    /* TODO: YOUR CODE HERE */
    if (mat == NULL)
        return;
    if (mat->ref_cnt > 1)
    {                      //has slices
        mat->ref_cnt -= 1; // !!!thus the parent matrix has no reference to itself!!!
    }
    else
    { // ref_cnt <= 1, no slices
        if (mat->parent == NULL)
        { // is not slice
            free(mat->data);
            free(mat);
        }
        else
        { // is slice
            mat->parent->ref_cnt -= 1;
            if (mat->parent->ref_cnt == 0) // if parent has no ref to anyone
            {
                deallocate_matrix(mat->parent);
            }
            free(mat);
        }
    }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid.
 */
double get(matrix *mat, int row, int col)
{
    /* TODO: YOUR CODE HERE */
    return mat->data[row * mat->cols + col];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid
 */
void set(matrix *mat, int row, int col, double val)
{
    /* TODO: YOUR CODE HERE */
    mat->data[row * mat->cols + col] = val;
}

/*
 * Sets all entries in mat to val
 */
void fill_matrix(matrix *mat, double val)
{
    /* TODO: YOUR CODE HERE */
    long size = (long)(mat->cols * mat->rows);
#pragma omp parallel for
    for (int i = 0; i < size; ++i)
    {
        mat->data[i] = val;
    }
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2)
{
    /* TODO: YOUR CODE HERE */
    if (result->rows == mat1->rows && result->cols == mat1->cols && result->rows == mat2->rows && result->cols == mat2->cols)
    {
        int size = result->rows * result->cols;
        double *result_d = result->data;
        double *mat1_d = mat1->data;
        double *mat2_d = mat2->data;
        __m256d d1;
        __m256d d2;
        __m256d res1;
        __m256d res2;
        __m256d res3;
        __m256d res4;
        int unroll = 4;
        int stride = unroll * 4;
        omp_set_num_threads(8);
        int ompSize = size / stride * stride - stride;
#pragma omp parallel private(d1, d2, res1, res2, res3, res4)
        {
#pragma omp for
            for (int i = 0; i <= ompSize; i += stride)
            {
                d1 = _mm256_loadu_pd(&mat1_d[i]);
                d2 = _mm256_loadu_pd(&mat2_d[i]);
                res1 = _mm256_add_pd(d1, d2);

                d1 = _mm256_loadu_pd(&mat1_d[i + 4]);
                d2 = _mm256_loadu_pd(&mat2_d[i + 4]);
                res2 = _mm256_add_pd(d1, d2);

                d1 = _mm256_loadu_pd(&mat1_d[i + 8]);
                d2 = _mm256_loadu_pd(&mat2_d[i + 8]);
                res3 = _mm256_add_pd(d1, d2);

                d1 = _mm256_loadu_pd(&mat1_d[i + 12]);
                d2 = _mm256_loadu_pd(&mat2_d[i + 12]);
                res4 = _mm256_add_pd(d1, d2);

                _mm256_storeu_pd(&result_d[i], res1);
                _mm256_storeu_pd(&result_d[i + 4], res2);
                _mm256_storeu_pd(&result_d[i + 8], res3);
                _mm256_storeu_pd(&result_d[i + 12], res4);
            }
        }
        for (int k = size / stride * stride; k < size; ++k)
        {
            result_d[k] = mat1_d[k] + mat2_d[k];
        }
        return 0;
    }
    return -1;
}

/*
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2)
{
    /* TODO: YOUR CODE HERE */
    if (result->rows == mat1->rows && result->cols == mat1->cols && result->rows == mat2->rows && result->cols == mat2->cols)
    {
        int size = result->rows * result->cols;
        double *result_d = result->data;
        double *mat1_d = mat1->data;
        double *mat2_d = mat2->data;
        __m256d d1;
        __m256d d2;
        __m256d res1;
        __m256d res2;
        __m256d res3;
        __m256d res4;
        int unroll = 4;
        int stride = unroll * 4;
        omp_set_num_threads(8);
        int ompSize = size / stride * stride - stride;
#pragma omp parallel private(d1, d2, res1, res2, res3, res4)
        {
#pragma omp for
            for (int i = 0; i <= ompSize; i += stride)
            {
                d1 = _mm256_loadu_pd(&mat1_d[i]);
                d2 = _mm256_loadu_pd(&mat2_d[i]);
                res1 = _mm256_sub_pd(d1, d2);

                d1 = _mm256_loadu_pd(&mat1_d[i + 4]);
                d2 = _mm256_loadu_pd(&mat2_d[i + 4]);
                res2 = _mm256_sub_pd(d1, d2);

                d1 = _mm256_loadu_pd(&mat1_d[i + 8]);
                d2 = _mm256_loadu_pd(&mat2_d[i + 8]);
                res3 = _mm256_sub_pd(d1, d2);

                d1 = _mm256_loadu_pd(&mat1_d[i + 12]);
                d2 = _mm256_loadu_pd(&mat2_d[i + 12]);
                res4 = _mm256_sub_pd(d1, d2);

                _mm256_storeu_pd(&result_d[i], res1);
                _mm256_storeu_pd(&result_d[i + 4], res2);
                _mm256_storeu_pd(&result_d[i + 8], res3);
                _mm256_storeu_pd(&result_d[i + 12], res4);
            }
        }
        for (int k = size / stride * stride; k < size; ++k)
        {
            result_d[k] = mat1_d[k] - mat2_d[k];
        }
        return 0;
    }
    return -1;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2)
{
    /* TODO: YOUR CODE HERE */
    int mat1_row = mat1->rows;
    int mat1_col = mat1->cols;
    int mat2_col = mat2->cols;
    if (result->rows != mat1_row || result->cols != mat2_col || mat1_col != mat2->rows)
    {
        return -1;
    }
    if (result != mat1 && result != mat2)
    {
        fill_matrix(result, 0.0);
        int i, j, k;
        for (j = 0; j < mat1_row; j++)
            for (k = 0; k < mat1_col; k++)
                for (i = 0; i < mat2_col; i++)
                    result->data[i + j * mat2_col] += mat1->data[k + j * mat1_col] * mat2->data[i + k * mat2_col];
    }
    else
    { // supports A = AB type multiplication
        matrix *tmp;
        if (allocate_matrix(&tmp, result->rows, result->cols) == 0)
        {
            mul_matrix(tmp, mat1, mat2);
            copy_matrix(result, tmp);
            free(tmp->data);
            free(tmp);
            return 0;
        }
        else return -1;
    }

    return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 */
int pow_matrix(matrix *result, matrix *mat, int pow)
{
    /* TODO: YOUR CODE HERE */
    if (mat->rows != mat->cols || pow < 0 || result->cols != mat->cols || result->rows != mat->rows)
    {
        return -1;
    }
    /*
    {
        set_eye(result);
        for (int i = 0; i < pow; ++i)
        {
            mul_matrix(result, result, mat);
        }
        return 0;
    }
    */
    int max_num_of_threads = omp_get_max_threads();
    matrix *results[max_num_of_threads];
    set_eye(result);
    for (int i = 0; i < max_num_of_threads; ++i)
    {
        int allocate_failed = allocate_matrix(&results[i], mat->rows, mat->cols);
        if (allocate_failed)
        {
            return -1;
        }
        set_eye(results[i]);
    }
#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < pow; i++)
            mul_matrix(results[omp_get_thread_num()], results[omp_get_thread_num()], mat);
#pragma omp critical
        for (int i = 0; i < max_num_of_threads; ++i)
        {
            mul_matrix(result, result, results[i]);
        }
    }
    
    return 0;
}

/*
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int neg_matrix(matrix *result, matrix *mat)
{
    /* TODO: YOUR CODE HERE */
    if (result->cols != mat->cols || result->rows != mat->rows)
    {
        return -1;
    }
    int size = result->rows * result->cols;
    double *result_d = result->data;
    double *mat1_d = mat->data;
    __m256d d1;
    __m256d res1;
    __m256d res2;
    __m256d res3;
    __m256d res4;
    __m256d mask = _mm256_set1_pd(-1.0);
    int unroll = 4;
    int stride = unroll * 4;
    omp_set_num_threads(8);
    int ompSize = size / stride * stride - stride;
#pragma omp parallel private(d1, res1, res2, res3, res4)
    {
#pragma omp for
        for (int i = 0; i <= ompSize; i += stride)
        {
            d1 = _mm256_loadu_pd(&mat1_d[i]);
            res1 = _mm256_mul_pd(d1, mask);

            d1 = _mm256_loadu_pd(&mat1_d[i + 4]);
            res2 = _mm256_mul_pd(d1, mask);

            d1 = _mm256_loadu_pd(&mat1_d[i + 8]);
            res3 = _mm256_mul_pd(d1, mask);

            d1 = _mm256_loadu_pd(&mat1_d[i + 12]);
            res4 = _mm256_mul_pd(d1, mask);

            _mm256_storeu_pd(&result_d[i], res1);
            _mm256_storeu_pd(&result_d[i + 4], res2);
            _mm256_storeu_pd(&result_d[i + 8], res3);
            _mm256_storeu_pd(&result_d[i + 12], res4);
        }
    }
    for (int k = size / stride * stride; k < size; ++k)
    {
        result_d[k] = -1.0 * mat1_d[k];
    }
    return 0;
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int abs_matrix(matrix *result, matrix *mat)
{
    /* TODO: YOUR CODE HERE */
    if (result->cols != mat->cols || result->rows != mat->rows)
    {
        return -1;
    }
    int size = result->rows * result->cols;
    double *result_d = result->data;
    double *mat1_d = mat->data;
    __m256d d1;
    __m256d res1;
    __m256d res2;
    __m256d res3;
    __m256d res4;
    __m256d mask;
    __m256d negMask;
    __m256d zero = _mm256_set1_pd(0.0);
    __m256d neg = _mm256_set1_pd(-1.0);

    int unroll = 4;
    int stride = unroll * 4;
    omp_set_num_threads(8);
    int ompSize = size / stride * stride - stride;
#pragma omp parallel private(d1, res1, res2, res3, res4)
    {
#pragma omp for
        for (int i = 0; i <= ompSize; i += stride)
        {
            d1 = _mm256_loadu_pd(&mat1_d[i]);
            mask = _mm256_cmp_pd(zero, d1, 14);
            negMask = _mm256_and_pd(neg, mask);
            res1 = _mm256_mul_pd(d1, negMask);
            res1 = _mm256_max_pd(d1, res1);

            d1 = _mm256_loadu_pd(&mat1_d[i + 4]);
            mask = _mm256_cmp_pd(zero, d1, 14);
            negMask = _mm256_and_pd(neg, mask);
            res2 = _mm256_mul_pd(d1, negMask);
            res2 = _mm256_max_pd(d1, res2);

            d1 = _mm256_loadu_pd(&mat1_d[i + 8]);
            mask = _mm256_cmp_pd(zero, d1, 14);
            negMask = _mm256_and_pd(neg, mask);
            res3 = _mm256_mul_pd(d1, negMask);
            res3 = _mm256_max_pd(d1, res3);

            d1 = _mm256_loadu_pd(&mat1_d[i + 12]);
            mask = _mm256_cmp_pd(zero, d1, 14);
            negMask = _mm256_and_pd(neg, mask);
            res4 = _mm256_mul_pd(d1, negMask);
            res4 = _mm256_max_pd(d1, res4);

            _mm256_storeu_pd(&result_d[i], res1);
            _mm256_storeu_pd(&result_d[i + 4], res2);
            _mm256_storeu_pd(&result_d[i + 8], res3);
            _mm256_storeu_pd(&result_d[i + 12], res4);
        }
    }
    int tmp;
    for (int k = size / stride * stride; k < size; ++k)
    {
        tmp = mat1_d[k];
        result_d[k] = tmp > 0 ? tmp : -1.0 * tmp;
    }
    return 0;
}

void set_eye(matrix *mat)
{
    int size = mat->rows * mat->cols;
#pragma omp parallel for
    for (int i = 0; i < size; ++i)
    {
        if (i / mat->cols == i % mat->cols)
        {
            mat->data[i] = 1.0;
        }
        else
        {
            mat->data[i] = 0.0;
        }
    }
}
int copy_matrix(matrix *dst, matrix *src)
{
    if (src->cols != dst->cols || src->rows != dst->rows)
    {
        return -1;
    }
    int size = dst->rows * dst->cols;
#pragma omp parallel for
    for (int i = 0; i < size; ++i)
    {
        dst->data[i] = src->data[i];
    }
    return 0;
}