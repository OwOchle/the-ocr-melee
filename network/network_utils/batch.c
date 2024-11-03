#include "batch.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Batch *batch_new(uint16_t batchSize, size_t input_size, size_t output_size)
{
    Batch *batch = malloc(sizeof(Batch));
    if (!batch)
        return NULL;

    batch->batchSize = batchSize;
    batch->layers = malloc(batchSize * sizeof(BatchLayer *));
    if (!batch->layers)
    {
        free(batch);
        return NULL;
    }

    for (size_t i = 0; i < batchSize; i++)
    {
        batch->layers[i] = malloc(sizeof(BatchLayer));
        if (!batch->layers[i])
        {
            // Libérer les couches déjà alloué si probs
            for (size_t j = 0; j < i; j++)
            {
                free(batch->layers[j]->inputData);
                free(batch->layers[j]->outputData);
                free(batch->layers[j]);
            }
            free(batch->layers);
            free(batch);
            return NULL;
        }

        batch->layers[i]->inputData = malloc(input_size * sizeof(float));
        batch->layers[i]->outputData = malloc(output_size * sizeof(float));
        if (!batch->layers[i]->inputData || !batch->layers[i]->outputData)
        {
            // Encore, libérer en cas de malloc qui rate
            free(batch->layers[i]->inputData);
            free(batch->layers[i]->outputData);
            free(batch->layers[i]);
            for (size_t j = 0; j < i; j++)
            {
                free(batch->layers[j]->inputData);
                free(batch->layers[j]->outputData);
                free(batch->layers[j]);
            }
            free(batch->layers);
            free(batch);
            return NULL;
        }
    }
    return batch;
}

void batch_free(Batch *batch)
{
    if (!batch)
        return;

    for (size_t i = 0; i < batch->batchSize; i++)
    {
        if (batch->layers[i])
        {
            free(batch->layers[i]->inputData);
            free(batch->layers[i]->outputData);
            free(batch->layers[i]);
        }
    }
    free(batch->layers);
    free(batch);
}

void batch_shuffle(Batch *batch)
{
    for (uint16_t i = batch->batchSize - 1; i > 0; i--)
    {
        uint16_t j = (uint16_t)(rand() % (i + 1));

        BatchLayer *temp = batch->layers[i];
        batch->layers[i] = batch->layers[j];
        batch->layers[j] = temp;
    }
}
