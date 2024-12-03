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

        if (ai <= 0.0)
            ai = 1e-10;
        if (ai >= 1.0)
            ai = 1.0 - 1e-10;

        float term1 = -yi * log(ai);
        float term2 = -(1.0 - yi) * log(1.0 - ai);

        if (isnan(term1) || isinf(term1))
            term1 = 0.0;
        if (isnan(term2) || isinf(term2))
            term2 = 0.0;

        cost += term1 + term2;
    }

    return cost;
}

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
