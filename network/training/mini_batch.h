#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "../network.h"
#include "backpropagation.h"

typedef struct MiniBatchLayer
{
    uint16_t inputDataSize;
    float *inputData; // array
    uint16_t outputDataSize;
    float *outputData; // array
} MiniBatchLayer;

typedef struct MiniBatch
{
    uint16_t batchSize;
    MiniBatchLayer **layers;
} MiniBatch;

int update_mini_batch(
		Network *network, MiniBatch *mini_batch, float eta, float lambda,
    int total_training_size
);

/**
 * Creates a new empty mini_batch.
 * @param batchSize The number of layers in the mini_batch.
 * @param input_size The size of the inputs arrays.
 * Should be the size of the network `entryCount`.
 * @param output_size The size of the outputs arrays.
 * Should be the size of the last element of the network `nodePerlayers`.
 * @return A pointer to the newly created mini_batch.
 */
MiniBatch *
mini_batch_new(uint16_t batchSize, size_t input_size, size_t output_size);

/**
 * Frees the mini_batch.
 *
 * @note Using the mini_batch after it has been freed is
 * considered an undefined behavior.
 */
void mini_batch_free(MiniBatch *mini_batch);
