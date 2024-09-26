#include "arrays.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void array_print(size_t size, const uint16_t array[])
{
    if (size == 0)
    {
        printf("[]\n");
        return;
    }

    printf("[");

    for (size_t i = 0; i < size - 1; i++)
    {
        printf("%hi, ", array[i]);
    }

    printf("%hi]\n", array[size - 1]);
}

void array_float_print(size_t size, const float array[])
{
    if (size == 0)
    {
        printf("[]\n");
        return;
    }

    printf("[");

    for (size_t i = 0; i < size - 1; i++)
    {
        printf("%f, ", array[i]);
    }

    printf("%f]\n", array[size - 1]);
}

float array_get_as_matrix(
    const float array[], size_t rowLength, size_t row, size_t column
)
{
    return array[row * rowLength + column];
}

float *array_get_as_matrix_ptr(
    float array[], size_t rowLength, size_t row, size_t column
)
{
    return array + (row * rowLength) + column;
}