#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "../network.h"

typedef struct GradiantLayer
{
    float *weights;
    uint16_t nodeCount;
    float *bias;
} GradiantLayer;

typedef struct GradiantData
{
    uint16_t entryCount;
    char layerCount;
    GradiantLayer **layers;
} GradiantData;

/**
 * Compares the input data
 * (for example, the raster representation of an image)
 * with the desired output data
 * (for exemple, the classification between letters),
 * returns a tuple that can be used by the gradiant
 */
GradiantData *backprop(
    const Network *network, const size_t width, const float training_input[],
    const float desired_outputs[]
);
