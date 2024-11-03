#include "gradiant.h"

#include "../network.h"

#include <stdint.h>
#include <stdlib.h>

GradiantData *gradiant_new(const Network *network)
{

    const char layerCount = network->layerCount;
    const uint16_t input_size = network->entryCount;
    const uint16_t output_size = network->layers[layerCount - 1]->nodeCount;

    // Init Gradiant data (by passing values from the network)

    GradiantData *gradiant = malloc(sizeof(GradiantData));
    if (gradiant == NULL)
    {
        return NULL;
    }

    gradiant->entryCount = input_size;
    gradiant->layerCount = layerCount;
    gradiant->layers = calloc(layerCount, sizeof(GradiantLayer *));

    if (network->layers == NULL)
    {
        return NULL;
    }

    for (char i = 0; i < layerCount; i++)
    {
        const int nodeCount = network->layers[i]->nodeCount;
        GradiantLayer *layer = malloc(sizeof(GradiantLayer));

        if (layer == NULL)
        {
            return NULL;
        }

        gradiant->layers[i] = layer;

        layer->nodeCount = nodeCount;

        int pastLayerCount;
        if (i == 0)
        {
            pastLayerCount = input_size;
        }
        else
        {
            pastLayerCount = network->layers[i - 1]->nodeCount;
        }

        layer->weights = calloc(
            network->layers[i]->nodeCount * pastLayerCount, sizeof(float)
        );
        if (layer->weights == NULL)
        {
            return NULL;
        }

        layer->bias = calloc(nodeCount, sizeof(float));
        if (layer->bias == NULL)
        {
            return NULL;
        }
    }

    return gradiant;
}

void gradiant_free(GradiantData *gradiant)
{
    for (char l = 0; l < gradiant->layerCount; l++)
    {
        GradiantLayer *layer = gradiant->layers[l];

        free(layer->bias);
        free(layer->weights);
        free(layer);
    }

    free(gradiant->layers);
    free(gradiant);
}
