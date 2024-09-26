#pragma once
#include <stdint.h>
#include <stdlib.h>

/**
 * Prints an array of uint16_t
 */
void array_print(size_t size, const uint16_t array[]);

/**
 * Prints an array of float
 */
void array_float_print(size_t size, const float array[]);

/**
 * Gets the element at position [y][x] of the matrix in row before notation,
 * y × width + x
 */
float array_get_as_matrix(
    const float array[], size_t width, size_t x, size_t y
);

/**
 * Gets the pointer at position [y][x] of the matrix in row before notation,
 * y × width + x
 */
float *array_get_as_matrix_ptr(float array[], size_t width, size_t x, size_t y);