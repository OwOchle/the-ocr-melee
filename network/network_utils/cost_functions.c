#include "cost_functions.h"

#include <math.h>

float cross_entropy_cost(
    size_t size, const float *activation, const float *desired_data
)
{
    float cost = 0.0;

    for (size_t i = 0; i < size; i++)
    {
        float ai = activation[i];
        float yi = desired_data[i];

        if (ai == 0.0)
            ai = 1e-10;
        if (ai == 1.0)
            ai = 1.0 - 1e-10;

        cost += -yi * log(ai) - (1 - yi) * log(1 - ai);
    }

    return cost;
}

/**
 * Prints an array using the appropriate method for type
 */
void cross_entropy_delta(
    size_t size, const float *activation, const float *desired_data,
    float *output
)
{
    for (size_t i = 0; i < size; i++)
    {
        output[i] = activation[i] - desired_data[i];
    }
}
