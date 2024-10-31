#include "batch.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Batch *mini_batch_new(uint16_t batchSize, size_t input_size, size_t output_size)
{
    Batch *mini_batch = malloc(sizeof(Batch));
    if (!mini_batch)
        return NULL;

    mini_batch->batchSize = batchSize;
    mini_batch->layers = malloc(batchSize * sizeof(BatchLayer *));
    if (!mini_batch->layers)
    {
        free(mini_batch);
        return NULL;
    }

    for (size_t i = 0; i < batchSize; i++)
    {
        mini_batch->layers[i] = malloc(sizeof(BatchLayer));
        if (!mini_batch->layers[i])
        {
            // Libérer les couches déjà alloué si probs
            for (size_t j = 0; j < i; j++)
            {
                free(mini_batch->layers[j]->inputData);
                free(mini_batch->layers[j]->outputData);
                free(mini_batch->layers[j]);
            }
            free(mini_batch->layers);
            free(mini_batch);
            return NULL;
        }

        mini_batch->layers[i]->inputData = malloc(input_size * sizeof(float));
        mini_batch->layers[i]->outputData = malloc(output_size * sizeof(float));
        if (!mini_batch->layers[i]->inputData ||
            !mini_batch->layers[i]->outputData)
        {
            // Encore, libérer en cas de malloc qui rate
            free(mini_batch->layers[i]->inputData);
            free(mini_batch->layers[i]->outputData);
            free(mini_batch->layers[i]);
            for (size_t j = 0; j < i; j++)
            {
                free(mini_batch->layers[j]->inputData);
                free(mini_batch->layers[j]->outputData);
                free(mini_batch->layers[j]);
            }
            free(mini_batch->layers);
            free(mini_batch);
            return NULL;
        }
    }
    return mini_batch;
}

void mini_batch_free(Batch *mini_batch)
{
    if (!mini_batch)
        return;

    for (size_t i = 0; i < mini_batch->batchSize; i++)
    {
        if (mini_batch->layers[i])
        {
            free(mini_batch->layers[i]->inputData);
            free(mini_batch->layers[i]->outputData);
            free(mini_batch->layers[i]);
        }
    }
    free(mini_batch->layers);
    free(mini_batch);
}
