#include <stdlib.h>
#include <string.h>

#include "batch_conversion.h"
#include "network/network_utils/batch.h"
#include "utils/verbose.h"

float *to_float_array(size_t size, uchar *array)
{
    if (array == NULL)
        return NULL;

    float *out = calloc(size, sizeof(float));

    if (out == NULL)
        return NULL;

    for (size_t i = 0; i < size; i++)
    {
        out[i] = array[i] / 255.0f;
    }

    return out;
}

Batch *images_to_batch(size_t image_count, InputImage *images)
{
    Batch *batch = batch_new(image_count, IMAGE_SIZE * IMAGE_SIZE, 26);

    if (batch == NULL)
    {
        return NULL;
    }

    verbose_printf("Batch successfully allocated\n");

    for (size_t i = 0; i < image_count; i++)
    {
        batch->layers[i]->inputData = to_float_array(IMAGE_SIZE * IMAGE_SIZE, images[i].image);

        char expected_index = images[i].character - 'A';

        // verbose_printf("Expecting '%c' at index '%hhu'\n", images[i].character, expected_index);

        batch->layers[i]->outputData = calloc(26, sizeof(float));

        batch->layers[i]->outputData[expected_index] = 1.0f;
    }

    // verbose_printf("Batch conversion done\n");

    return batch;
}