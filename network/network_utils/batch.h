#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "../network.h"

typedef struct BatchLayer
{
    float *inputData;  // array
    float *outputData; // array
} BatchLayer;

typedef struct Batch
{
    uint16_t batchSize;
    BatchLayer **layers;
} Batch;

/**
 * Creates a new empty batch.
 * @param batchSize The number of layers in the batch.
 * @param input_size The size of the inputs arrays.
 * Should be the size of the network `entryCount`.
 * @param output_size The size of the outputs arrays.
 * Should be the size of the last element of the network `nodePerlayers`.
 * @return A pointer to the newly created batch.
 */
Batch *
mini_batch_new(uint16_t batchSize, size_t input_size, size_t output_size);

/**
 * Frees the batch.
 *
 * @note Using the batch after it has been freed is
 * considered an undefined behavior.
 */
void mini_batch_free(Batch *mini_batch);
