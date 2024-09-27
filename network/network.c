#include "network.h"
#include "utils/matrix.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Network *
network_new(char layerCount, uint16_t *nodesPerLayer, uint16_t entryCount)
{
    if (layerCount < 2)
        return NULL;

    Network *network = malloc(sizeof(Network));

    if (network == NULL)
        return NULL;

    network->entryCount = entryCount;
    network->layerCount = layerCount;
    network->layers = calloc(layerCount, sizeof(Layer *));

    if (network->layers == NULL)
        return NULL;

    for (char i = 0; i < layerCount; i++)
    {
        int nodeCount = nodesPerLayer[i];
        Layer *layer = malloc(sizeof(Layer));

        if (layer == NULL)
            return NULL;

        network->layers[i] = layer;

        layer->nodeCount = nodeCount;

        int pastLayerCount;
        if (i == 0)
        {
            pastLayerCount = entryCount;
        }
        else
        {
            pastLayerCount = network->layers[i - 1]->nodeCount;
        }

        layer->weights = calloc(pastLayerCount * nodeCount, sizeof(float));
        if (layer->weights == NULL)
            return NULL;

        layer->bias = calloc(nodeCount, sizeof(float));
        if (layer->bias == NULL)
            return NULL;
    }

    return network;
}

void network_init_flat(Network *network)
{
    for (char l = 0; l < network->layerCount; l++)
    {
        Layer *layer = network->layers[l];

        int prev;
        if (l == 0)
        {
            prev = network->entryCount;
        }
        else
        {
            prev = network->layers[l - 1]->nodeCount;
        }

        for (int n = 0; n < layer->nodeCount * prev; n++)
        {
            layer->weights[n] = 0.5f;
        }

        for (int n = 0; n < layer->nodeCount; n++)
        {
            layer->bias[n] = 0.5f;
        }
    }
}

void network_print(Network *network)
{
    printf("struct Network {\n");
    printf("\tentryCount -> %hi\n", network->entryCount);
    printf("\tlayerCount -> %hhi\n", network->layerCount);
    printf("\tnodesPerLayer -> [");

    for (int i = 0; i < network->layerCount - 1; i++)
    {
        printf("%hi, ", network->layers[i]->nodeCount);
    }

    printf("%hi]\n", network->layers[network->layerCount - 1]->nodeCount);
    printf("}\n");
}

float *network_apply(Network *network, float *input)
{
    float *mat = input;
    uint16_t prevSize;
    for (char l = 0; l < network->layerCount; l++)
    {
        if (l == 0)
        {
            prevSize = network->entryCount;
        }
        else
        {
            prevSize = network->layers[l - 1]->nodeCount;
        }

        float *tmp = matrix_multiply(
            prevSize, 1, mat, network->layers[l]->nodeCount, prevSize, network->layers[l]->weights
        );

        free(mat);
        mat = tmp;

        if (mat == NULL) 
            return NULL;

        matrix_add(
            network->layers[l]->nodeCount, 1, mat,
            network->layers[l]->nodeCount, 1, network->layers[l]->bias
        );
    }

    return mat;
}

void network_free(Network *network)
{
    for (char l = 0; l < network->layerCount; l++)
    {
        Layer *layer = network->layers[l];

        free(layer->bias);
        free(layer->weights);
        free(layer);
    }

    free(network->layers);
    free(network);
}

uint16_t network_last_layer_count(Network *network)
{
    return network->layers[network->layerCount - 1]->nodeCount;
}