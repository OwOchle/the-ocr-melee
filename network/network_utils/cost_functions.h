#pragma once
#include <stdint.h>
#include <stdlib.h>

/**
 * Return the cost associated with an activation output and desired output.
 */
float cross_entropy_cost(
    size_t size, const float *activation, const float *desired_data
);

/**
 * Output the vector error delta from the output layer
 */
void cross_entropy_delta(
    size_t size, const float *activation, const float *desired_data,
    float *output
);
