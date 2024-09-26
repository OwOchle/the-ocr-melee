#include "network.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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

void network_print(Network *network) {
    printf("struct Network {\n");
    printf("\tentryCount -> %hi\n", network->entryCount);
    printf("\tlayerCount -> %hhi\n", network->layerCount);
    printf("\tnodesPerLayer -> [");

    for (int i = 0; i < network->layerCount - 1; i++) {
        printf("%hi, ", network->layers[i]->nodeCount);
    }

    printf("%hi]\n", network->layers[network->layerCount - 1]->nodeCount);
    printf("}\n");
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