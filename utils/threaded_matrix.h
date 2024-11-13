#pragma once

#include <stdlib.h>

void mat_th_init_threadpool(int thread_count);

float *mat_th_multiply(
    size_t width1, size_t height1, const float *mat1, size_t width2,
    size_t height2, const float *mat2
);

void mat_th_add(
    size_t width1, size_t height1, float *mat1, size_t width2, size_t height2,
    const float *mat2
);

void mat_th_destroy_threadpool();