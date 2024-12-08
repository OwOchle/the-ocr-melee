#include "array.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void array_uint16t_print(size_t size, const uint16_t array[])
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

void array_char_print(size_t size, const char array[])
{
    if (size == 0)
    {
        printf("[]\n");
        return;
    }

    printf("[");

    for (size_t i = 0; i < size - 1; i++)
    {
        printf("%c, ", array[i]);
    }

    printf("%c]\n", array[size - 1]);
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

size_t array_max_index_float(size_t size, const float array[])
{
    size_t max = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (array[i] > array[max])
        {
            max = i;
        }
    }

    return max;
}