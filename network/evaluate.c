#include <stdint.h>
#include <stdlib.h>

#include "../utils/matrix.h"
#include "network.h"
#include "network_utils/activation_functions.h"

typedef float *Matrix;
typedef float *Vector;

float *feedforward(Network *network, float *input_data)
{
    uint16_t layerCount = network->layerCount;

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
            pastNodeCount, nodeCount, weights, 1, pastNodeCount, input_data
        );
        if (dot == NULL)
        {
            return NULL;
        }

        matrix_add(1, nodeCount, dot, 1, nodeCount, bias);

        free(input_data);

        sigmoid(nodeCount, dot, input_data);
    }

    return input_data;
}
