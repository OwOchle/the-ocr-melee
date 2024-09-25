#pragma once
#include <stdint.h>

typedef struct Layer
{
    float
        *weights; // Matrix of layer-1 × layer as a list of node, in line order
    uint16_t nodeCount;
    float *bias; // List of bias of each node of the layer.
} Layer;

typedef struct Network
{
    uint16_t entryCount;
    char layerCount;
    Layer **layers; // List of layers*
} Network;

Network *
network_new(char layerCount, uint16_t *nodesPerLayer, uint16_t entryCount);

void network_init_flat(Network *network);

void network_free(Network *network);