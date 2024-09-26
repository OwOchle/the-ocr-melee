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

/**
 * Creates a new empty network.
 * @note Reading or using the network as is is considered an
 * undefined behavior
 * @param layerCount The number of layers in the network. The first/entry layer
 * is not counted
 * @param nodesPerLayer A list of uint16 describing for each layer
 * the number of nodes. Should be the size of `layerCount`.
 * @param entryCount The number of entry nodes/nodes on the first entry layer.
 * @return A pointer to the newly created network.
 */
Network *
network_new(char layerCount, uint16_t *nodesPerLayer, uint16_t entryCount);

/**
 * Prints network infos.
 */
void network_print(Network *network);

/**
 * Initialize the network with all weights and bias to 0.5
 */
void network_init_flat(Network *network);

/**
 * Frees the network. Freeing a network should not be made by hand and should
 * only occur with this method. 
 * 
 * @note Using the network after it has been freed is
 * considered an undefined behavior.
 */
void network_free(Network *network);