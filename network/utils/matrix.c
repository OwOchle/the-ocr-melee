#include "matrix.h"
#include "array.h"

#include <stdio.h>
#include <stdlib.h>

float *matrix_multiply(
    size_t width1, size_t height1, float *mat1, size_t width2, size_t height2,
    float *mat2
)
{
    if (width1 != height2)
    {
        return NULL;
    }

    float *res = calloc(height1 * width2, sizeof(float));

    for (size_t y = 0; y < height1; y++)
    {
        for (size_t x = 0; x < width2; x++)
        {
            for (size_t k = 0; k < height2; k++)
            {
                float *resPointer = array_get_as_matrix_ptr(res, width2, x, y);

                *resPointer += array_get_as_matrix(mat1, width1, k, y) *
                               array_get_as_matrix(mat2, width2, x, k);
            }
        }
    }

    return res;
}