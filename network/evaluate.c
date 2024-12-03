#include "evaluate.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils/array.h"
#include "../utils/matrix.h"
#include "network.h"
#include "network_utils/activation_functions.h"
#include "network_utils/cost_functions.h"
// #include "../utils/threaded_matrix.h"

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

float total_cost(Network *network, Batch *batch, float lambda)
{
    float cost = 0.0f;
    uint16_t batch_size = batch->batchSize;

    uint16_t output_size = network->layers[network->layerCount - 1]->nodeCount;
    for (size_t i = 0; i < batch_size; i++)
    {
        float *desired_output = batch->layers[i]->outputData;
        float *output = feedforward(network, batch->layers[i]->inputData);

        cost += cross_entropy_cost(output_size, output, desired_output) / batch_size;

        free(output);
    }

    float linear_norm = 0.0f;
    for (size_t i = 0; i < network->layerCount - 1; i++)
    {
        Layer *layer = network->layers[i];
        Layer *nextLayer = network->layers[i + 1];
        for (size_t j = 0; j < layer->nodeCount; j++)
        {
            for (size_t k = 0; k < nextLayer->nodeCount; k++)
            {
                float weight = array_get_as_matrix(layer->weights, nextLayer->nodeCount, j, k);
                linear_norm += weight * weight;
            }
        }
    }
    cost += 0.5f * (lambda / batch_size) * linear_norm;

    return cost;
}
