#include "arrays.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void array_print(size_t size, const uint16_t array[]) {
    if (size == 0) {
        printf("[]\n");
        return;
    }

    printf("[");
    
    for (size_t i = 0; i < size - 1; i++) {
        printf("%hi, ", array[i]);
    }

    printf("%hi]\n", array[size-1]);
}

float array_get_as_matrix(const float array[], size_t rowLength, size_t row, size_t column) {
    return array[row * rowLength + column];
}