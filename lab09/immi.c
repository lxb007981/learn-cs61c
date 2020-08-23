#include <immintrin.h>
#include <stdio.h>
int add_no_SSE(int size, int *first_array, int *second_array)
{
    for (int i = 0; i < size; ++i)
    {
        first_array[i] += second_array[i];
    }
}

int add_SSE(int size, int *first_array, int *second_array)
{
    int i = 0;
    for (; i + 4 <= size; i += 4)
    {
        // load 128-bit chunks of each array
        __m128i first_values = _mm_loadu_si128((__m128i *)&first_array[i]);
        __m128i second_values = _mm_loadu_si128((__m128i *)&second_array[i]);

        // add each pair of 32-bit integers in the 128-bit chunks
        first_values = _mm_add_epi32(first_values, second_values);

        // store 128-bit chunk to first array
        _mm_storeu_si128((__m128i *)&first_array[i], first_values);
    }
    // handle left-over
    for (; i < size; ++i)
    {
        first_array[i] += second_array[i];
    }
}
int main()
{
    /*__m128i values = _mm_setr_epi32(0x1234, 0x2345, 0x3456, 0x4567);
    int first_value = _mm_extract_epi32(values, 0);
    // first_value == 0x1234
    int second_value = _mm_extract_epi32(values, 1);
    // second_value == 0x2345

    int arrayA[4];
    _mm_storeu_si128((__m128i *)arrayA, values);
    // arrayA[0] == 0x1234
    // arrayA[1] == 0x2345
    // ...

    int arrayB[4] = {10, 20, 30, 40};
    values = _mm_loadu_si128((__m128i *)arrayB);
    // 10 == arrayB[0] == _mm_extract_epi32(values, 0)
    // 20 == arrayB[1] == _mm_extract_epi32(values, 1)
    // ...

    __m128i first_values = _mm_setr_epi32(10, 20, 30, 40);
    __m128i second_values = _mm_setr_epi32(5, 6, 7, 8);
    __m128i result_values = _mm_add_epi32(first_values, second_values);
    // _mm_extract_epi32(result_values, 0) == 15
    // _mm_extract_epi32(result_values, 1) == 26
    // _mm_extract_epi32(result_values, 2) == 37
    // _mm_extract_epi32(result_values, 3) == 48
    */
    int array1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int array2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    add_SSE(10, array1, array2);
    for (int i = 0; i < 10; ++i)
        printf("%d\n", array1[i]);
}