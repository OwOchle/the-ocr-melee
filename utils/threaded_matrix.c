#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "thpool.h"
#include "threaded_matrix.h"

threadpool gMulTp = NULL;
int gMulThreadCount = 0;
threadpool gAddTp = NULL;

void mat_th_init_threadpool(int thread_count)
{
    gMulTp = thpool_init(thread_count);
    gMulThreadCount = thread_count;
    gAddTp = thpool_init(thread_count);
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

    size_t row_start;
    size_t row_end;
};

void __mat_th_multiply_row(void *vdata)
{
    struct MatrixMulData *data = vdata;

    for (size_t row = data->row_start; row < data->row_end && row < data->height1; row++)
    {
        for (size_t x = 0; x < data->width2; x++)
        {
            for (size_t k = 0; k < data->height2; k++)
            {
                *(array_get_as_matrix_ptr(data->res, data->width2, x, row)) +=
                    array_get_as_matrix(data->mat1, data->width1, k, row) *
                    array_get_as_matrix(data->mat2, data->width2, x, k);
            }
        }
    }
}

float *mat_th_multiply(
    size_t width1, size_t height1, const float *mat1, size_t width2,
    size_t height2, const float *mat2
)
{
    if (width1 != height2)
    {
        return NULL;
    }

    if (thpool_num_threads_working(gMulTp) != 0)
    {
        thpool_wait(gMulTp);
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

    struct MatrixMulData *datas = malloc(height1 * sizeof(struct MatrixMulData));

    size_t per_call = height1 / gMulThreadCount;

    for (size_t y = 0; y < height1; y += per_call)
    {
        memcpy(&datas[y], &template, sizeof(struct MatrixMulData));

        datas[y].row_start = y;
        datas[y].row_end = y + per_call;

        thpool_add_work(gMulTp, __mat_th_multiply_row, &datas[y]);
    }
    
    thpool_wait(gMulTp);

    free(datas);

    return res;
}

struct MatrixAddData
{
    size_t width1;
    size_t height1;
    float *mat1;
    size_t width2;
    size_t height2;
    const float *mat2;

    size_t i;
};

void __mat_th_add_row(void *vdata)
{
    struct MatrixAddData *data = vdata;

    for (size_t x = 0; x < data->width1; x++)
    {
        *(array_get_as_matrix_ptr(data->mat1, data->width1, x, data->i)) += \
        array_get_as_matrix(data->mat2, data->width2, x, data->i);
    }
}

void __mat_th_add_column(void *vdata)
{
    struct MatrixAddData *data = vdata;

    for (size_t y = 0; y < data->height1; y++)
    {
        *(array_get_as_matrix_ptr(data->mat1, data->width1, data->i, y)) += \
            array_get_as_matrix(data->mat2, data->width2, data->i, y);
    }
}

void mat_th_add(
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

    if (thpool_num_threads_working(gAddTp) != 0)
    {
        thpool_wait(gAddTp);
    }

    struct MatrixAddData template;

    template.height1 = height1;
    template.height2 = height2;
    template.width1 = width1;
    template.width2 = width2;
    template.mat1 = mat1;
    template.mat2 = mat2;

    struct MatrixAddData *datas = malloc((height1 < width1 ? width1 : height1) * sizeof(struct MatrixAddData));

    for (size_t i = 0; i < (height1 < width1 ? width1 : height1); i++)
    {
        memcpy(&datas[i], &template, sizeof(struct MatrixAddData));

        datas[i].i = i;

        thpool_add_work(gAddTp, (height1 < width1 ? __mat_th_add_column : __mat_th_add_row), &datas[i]);
    }

    thpool_wait(gAddTp);
}

void mat_th_destroy_threadpool()
{
    thpool_destroy(gMulTp);
    thpool_destroy(gAddTp);
}