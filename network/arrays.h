#pragma once
#include <stdint.h>
#include <stdlib.h>

void int16_array_print(size_t size, uint16_t array[]);

float array_get_as_matrix(const float array[], size_t rowLength, size_t row, size_t column);