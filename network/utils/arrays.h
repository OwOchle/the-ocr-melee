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
 * Gets the element at position [row][column] of the matrix in row before notation,
 * row × rowLength + column
 */
float array_get_as_matrix(const float array[], size_t rowLength, size_t row, size_t column);

/**
 * Gets the pointer at position [row][column] of the matrix in row before notation,
 * row × rowLength + column
 */
float *array_get_as_matrix_ptr(float array[], size_t rowLength, size_t row, size_t column);