#pragma once
#include <stdint.h>
#include <stdlib.h>

float *matrix_multiply(
    size_t width1, size_t height1, float *mat1, size_t width2, size_t height2,
    float *mat2
);