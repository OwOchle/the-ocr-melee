#include "matrix.h"
#include "array.h"
#include "verbose.h"

#include <stdio.h>
#include <stdlib.h>

float *matrix_multiply(
    size_t width1, size_t height1, const float *mat1, size_t width2,
    size_t height2, const float *mat2
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

void matrix_add(
    size_t width1, size_t height1, float *mat1, size_t width2, size_t height2,
    const float *mat2
)
{
    if (width1 != width2)
    {
        return;
    }

    if (height1 != height2)
    {
        return;
    }

    for (size_t y = 0; y < height1; y++)
    {
        for (size_t x = 0; x < width1; x++)
        {
            float *ptr = array_get_as_matrix_ptr(mat1, width1, x, y);
            *ptr += array_get_as_matrix(mat2, width2, x, y);
        }
    }
}

void matrix_transpose(
    size_t width, size_t height, const float *mat_input, float *mat_output
)
{
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            *array_get_as_matrix_ptr(mat_output, height, y, x) =
                array_get_as_matrix(mat_input, width, x, y);
        }
    }
}

float *matrix_multiply_array(
    size_t width1, size_t height1, const float *mat1, size_t vec_len, const float *vec
)
{
    if (width1 != vec_len)
    {
        return NULL;
    }

    float *res = calloc(height1, sizeof(float));
    if (res == NULL)
    {
        return NULL;
    }

    for (size_t y = 0; y < height1; y++)
    {
        for (size_t x = 0; x < width1; x++)
        {
            res[y] += array_get_as_matrix(mat1, width1, x, y) * vec[x];
        }
    }

    return res;
}

void matrix_add_array(size_t width, size_t height, float *mat, const float *vec)
{
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            float *ptr = array_get_as_matrix_ptr(mat, width, x, y);
            *ptr += vec[x];
        }
    }
}

void verbose_print_matrix_uchar(unsigned char *mat, size_t width, size_t height)
{
    if (height == 1)
    {
        verbose_printf("[");
        for (int x = 0; x < width-1; x++)
        {
            verbose_printf("%3d ", mat[x]);
        }
        verbose_printf("%3d]\n", mat[width-1]);
    }
    else
    {
        verbose_printf("┏");
        for (size_t x = 0; x < (((width * 3) + width) - 1); x++) verbose_printf(" ");
        verbose_printf("┓\n");
        
        for (size_t y = 0; y < height; y++)
        {
            verbose_printf("┃");
            for (size_t x = 0; x < width-1; x++)
            {
                verbose_printf("%3d ", mat[(y * width) + x]);
            }
            
            verbose_printf("%3d┃\n", mat[(y * width) + (width-1)]);
        }
        
        verbose_printf("┗");
        for (size_t x = 0; x < (((width * 3) + width) - 1); x++) verbose_printf(" ");
        verbose_printf("┛\n");
    }
}