#pragma once
#include <stdint.h>
#include <stdlib.h>

/**
 * Compares the input data 
 * (for example, the raster representation of an image)
 * with the desired output data 
 * (in this case, the classification between letters),
 * returns a tuple that can be used for the gradiant
 */
void backprop(
    size_t size, const float training_input[], const float desired_outputs[],
    float bias_gradiants[], float weigth_gradiants[]
);
