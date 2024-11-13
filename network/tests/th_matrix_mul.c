#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils/threaded_matrix.h"
#include "utils/matrix.h"

float float_rand( float min, float max )
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

void fill_array(float *array, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        array[i] = float_rand(-5.0, 5.0);
    }
}

double time_diff(struct timespec start, struct timespec end)
{
    return (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

int main()
{
    srand((unsigned int) time(NULL));

    size_t width1 = 250;
    size_t width2 = 250;

    size_t height1 = 250;
    size_t height2 = 250; 

    float *mat1 = calloc(width1 * height1, sizeof(float));
    float *mat2 = calloc(width2 * height2, sizeof(float));

    fill_array(mat1, width1 * height1);
    fill_array(mat2, width2 * height2);

    mat_th_init_threadpool(12);

    struct timespec start_single;

    clock_gettime(CLOCK_MONOTONIC, &start_single);

    float *res_single = matrix_multiply(width1, height1, mat1, width2, height2, mat2);

    struct timespec end_single;

    clock_gettime(CLOCK_MONOTONIC, &end_single);

    printf("single: %f ms\n", time_diff(start_single, end_single));

    struct timespec start_th;

    clock_gettime(CLOCK_MONOTONIC, &start_th);

    float *res_th = mat_th_multiply_matrix(width1, height1, mat1, width2, height2, mat2);

    struct timespec end_th;

    clock_gettime(CLOCK_MONOTONIC, &end_th);

    printf("th: %f ms\n", time_diff(start_th, end_th));

    // verbose_print_matrix_float(res_single, height1, width2);

    // verbose_print_matrix_float(res_th, height1, width2);

    free(mat1);
    free(mat2);
    free(res_single);
    free(res_th);

    mat_th_destroy_threadpool();

    return 0;
}