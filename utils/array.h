#pragma once
#include <stdint.h>
#include <stdlib.h>

/**
 * Prints an array using the appropriate method for type
 */
#define array_print(size, array) _Generic((array), \
                                          char *: array_char_print, \
                                          uint16_t *: array_uint16t_print, \
                                          float *: array_float_print)(size, array)

/**
 * Gets the element at position [y][x] of the matrix in row before notation,
 * y × width + x
 */
#define array_get_as_matrix(array, width, x, y) array[y * width + x]

/**
 * Gets the pointer at position [y][x] of the matrix in row before notation,
 * y × width + x
 */
#define array_get_as_matrix_ptr(array, width, x, y) (&array[(y * width) + x])

/**
 * Prints an array of uint16_t
 */
void array_uint16t_print(size_t size, const uint16_t array[]);

/**
 * Prints an array of float
 */
void array_float_print(size_t size, const float array[]);

/**
 * Prints an array of char
 */
void array_char_print(size_t size, const char array[]);

size_t array_max_index_float(size_t size, const float array[]);