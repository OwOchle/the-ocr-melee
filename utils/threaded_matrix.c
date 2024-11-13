#include <stdlib.h>
#include <string.h>

#include "thpool.h"
#include "threaded_matrix.h"
#include "array.h"

threadpool tp = NULL;

void mat_th_init_threadpool(int thread_count)
{
    tp = thpool_init(thread_count);
}

struct MatrixMulData
{
    size_t width1;
    size_t height1;
    const float *mat1;
    size_t width2;
    size_t height2;
    const float *mat2;

    float *res;

    size_t row;
};

void __mat_th_multiply_row(void *vdata)
{
    struct MatrixMulData *data = vdata;

    for (size_t x = 0; x < data->width2; x++)
    {
        for (size_t k = 0; k < data->height2; k++)
        {
            float *resPointer = array_get_as_matrix_ptr(data->res, data->width2, x, data->row);

            *resPointer += array_get_as_matrix(data->mat1, data->width1, k, data->row) *
                            array_get_as_matrix(data->mat2, data->width2, x, k);
        }
    }

    free(vdata);
}

float *mat_th_multiply_matrix(
    size_t width1, size_t height1, const float *mat1, size_t width2,
    size_t height2, const float *mat2
)
{
    if (width1 != height2)
    {
        return NULL;
    }

    if (thpool_num_threads_working(tp) != 0)
    {
        thpool_wait(tp);
    }

    float *res = calloc(height1 * width2, sizeof(float));

    struct MatrixMulData template;

    template.height1 = height1;
    template.height2 = height2;
    template.width1 = width1;
    template.width2 = width2;
    template.mat1 = mat1;
    template.mat2 = mat2;

    template.res = res;

    for (size_t y = 0; y < height1; y++)
    {
        struct MatrixMulData *d = malloc(sizeof(struct MatrixMulData));

        memcpy(d, &template, sizeof(struct MatrixMulData));

        d->row = y;

        thpool_add_work(tp, __mat_th_multiply_row, d);
    }

    thpool_wait(tp);

    return res;    
}

void mat_th_destroy_threadpool()
{
    //
    thpool_destroy(tp);
}