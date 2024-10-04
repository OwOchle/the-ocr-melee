#include "utils/array.h"
#include "utils/matrix.h"

#include <stdio.h>
#include <stdlib.h>

void test1()
{
    float mat1[] = {1, 2, 3, 4, 5, 6};
    size_t width1 = 3;
    size_t height1 = 2;

    float mat2[] = {7, 8, 9, 10, 11, 12};
    size_t width2 = 2;
    size_t height2 = 3;

    float *res = matrix_multiply(width1, height1, mat1, width2, height2, mat2);

    array_float_print(height1 * width2, res);

    free(res);
}

int main()
{
    test1();
    return EXIT_SUCCESS;
}