#pragma once
#include <stdlib.h>


float *matrix_multiply(
    size_t width1, size_t height1, const float *mat1, size_t width2, size_t height2, const float *mat2
);

void matrix_add(
    size_t width1, size_t height1, float *mat1, size_t width2, size_t height2,
    const float *mat2
);

void matrix_transpose(
    size_t width, size_t height, const float *mat_input, float *mat_output
);

float *matrix_multiply_array(
    size_t width1, size_t height1, const float *mat1, size_t vec_len, const float *vec
);

void matrix_add_array(
    size_t width, size_t height, float *mat, const float *vec
);


void verbose_print_matrix_uchar(unsigned char *mat, size_t width, size_t height);

void verbose_print_matrix_float(float *mat, size_t width, size_t height);