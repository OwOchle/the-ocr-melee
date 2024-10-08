#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "../network.h"

typedef struct GradiantData
{
} GradiantData;

/**
 * Compares the input data
 * (for example, the raster representation of an image)
 * with the desired output data
 * (in this case, the classification between letters),
 * returns a tuple that can be used for the gradiant
 */
GradiantData *backprop(
    Network *network, size_t size, const float training_input[],
    const float desired_outputs[]
);
