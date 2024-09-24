#include "network.h"
#include <err.h>
#include <stdlib.h>

/**
 * depth: Layer count
 * nodePerLayer: List of layer nodes lengths
 */
Network *generate_flat_network(int depth, const int *nodePerLayer)
{
    if (depth < 2)
    {
        errx(1, "Minimum depth of network is 2, was : %d", depth);
    }

    Network *network = malloc(sizeof(Network));

    network->layerCount = depth;
    network->layers = malloc(depth * sizeof(Layer *));

    for (int iL = 0; iL < depth; iL++)
    {
        Layer *layer = malloc(sizeof(Layer));

        layer->nodeCount = nodePerLayer[iL];
        layer->nodes = malloc(layer->nodeCount * sizeof(Node *));
    }

    for (int iL = 0; iL < depth; iL++)
    {
        Layer *layer = network->layers[iL];
        for (int iNode = 0; iNode < layer->nodeCount; iNode++)
        {
            Node *node = malloc(sizeof(Node));

            node->bias = 0.5f;

            if (iL == 0)
            {
                node->entering = NULL;
            }
            else
            {
                node->entering = malloc(network->layers[iL-1]->nodeCount * sizeof(Edge *));
            } 
            
            if (iL == depth - 1)
            {
                node->exiting = NULL;
            }
            else
            {
                node->exiting = malloc(network->layers[iL+1]->nodeCount * sizeof(Edge *));
            }
        }
    }

    
}