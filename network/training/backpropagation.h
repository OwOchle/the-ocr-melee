#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "../network.h"
#include "../network_utils/gradiant.h"

/**
 * Compares the input data
 * (for example, the raster representation of an image)
 * with the desired output data
 * (for exemple, the classification between letters),
 * returns a `Gradiant` that can be used by to train the model
 */
GradiantData *backprop(
    const Network *network, const float training_input[],
    const float desired_outputs[]
);

float **alloc_z_matrix(const Network *network);
float **alloc_activation_matrix(const Network *network);

void free_z_matrix(float **z_matrix, const Network *network);
void free_activation_matrix(float **activation_matrix, const Network *network);
