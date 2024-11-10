#include "evaluate.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils/matrix.h"
#include "network.h"
#include "network_utils/activation_functions.h"

typedef float *Matrix;
typedef float *Vector;

float *feedforward(Network *network, const float *input_data)
{
    uint16_t layerCount = network->layerCount;
    size_t input_size = network->entryCount;
    size_t output_size = network->layers[network->layerCount - 1]->nodeCount;

    float *activation = calloc(input_size, sizeof(float));
    if (activation == NULL)
        return NULL;

    for (uint16_t i = 0; i < input_size; i++)
    {
        activation[i] = input_data[i];
    }

    for (size_t l = 0; l < layerCount; l++)
    {
        Matrix weights = network->layers[l]->weights;
        Vector bias = network->layers[l]->bias;

        uint16_t nodeCount = network->layers[l]->nodeCount;
        uint16_t pastNodeCount;
        if (l == 0)
        {
            pastNodeCount = network->entryCount;
        }
        else
        {
            pastNodeCount = network->layers[l - 1]->nodeCount;
        }

        Vector dot = matrix_multiply(
            pastNodeCount, nodeCount, weights, 1, pastNodeCount, activation
        );
        if (dot == NULL)
        {
            return NULL;
        }

        matrix_add(1, nodeCount, dot, 1, nodeCount, bias);

        float *temp = realloc(activation, nodeCount * sizeof(float));
        if (temp == NULL)
        {
            free(activation);
            return NULL;
        }

        activation = temp;

        if (l == layerCount - 1)
        {
            softmax(nodeCount, dot, activation);
        }
        else
        {
            sigmoid(nodeCount, dot, activation);
        }

        free(dot);
    }

    return activation;
}

int argmax(const float *array, uint16_t length)
{
    int max = 0;
    for (uint16_t i = 1; i < length; ++i)
    {
        if (array[i] > array[max])
        {
            max = i;
        }
    }
    return max;
}

int accuracy(Network *network, Batch *training_data)
{
    int count = 0;

    uint16_t output_size = network->layers[network->layerCount - 1]->nodeCount;

    for (uint16_t i = 0; i < training_data->batchSize; ++i)
    {
        BatchLayer *layer = training_data->layers[i];

        float *predicted_output = feedforward(network, layer->inputData);

        int predicted_max = argmax(predicted_output, output_size);
        int actual_max = argmax(layer->outputData, output_size);

        if (predicted_max == actual_max)
        {
            ++count;
        }

        free(predicted_output);
    }

    return count;
}

float binary_accuracy(Network *network, Batch *batch)
{
    int count = 0;

    for (uint16_t i = 0; i < batch->batchSize; i++)
    {
        BatchLayer *layer = batch->layers[i];

        printf(
            "Taking %.0f and %.0f as input => %.0f\n", layer->inputData[0],
            layer->inputData[1], layer->outputData[0]
        );
        float *predicted_output = feedforward(network, layer->inputData);
        printf("Network thinks it is %f\n", predicted_output[0]);

        int predicted_proba = predicted_output[0] >= 0.5 ? 1 : 0;

        int actual_proba = layer->outputData[0] >= 0.5 ? 1 : 0;

        if (predicted_proba == actual_proba)
        {
            count++;
        }

        free(predicted_output);
    }

    return count;
}
