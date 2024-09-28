#include "activation_functions.h"
#include <math.h>
#include <stdlib.h>

void sigmoid(size_t size, const float input[], float output[])
{
    for (size_t i = 0; i < size; i++)
    {
        output[i] = 1.0 / (1.0 + exp(-input[i]));
    }
}

void sigmoid_prime(size_t size, const float input[], float output[])
{
    for (size_t i = 0; i < size; i++)
    {
        float sigmoid = 1.0 / (1.0 + exp(-input[i]));
        output[i] = sigmoid * (1 - sigmoid);
    }
}

void softmax(size_t size, const float input[], float output[])
{
    float max = input[0]; // Numerical stability stuff
    for (size_t i = 1; i < size; ++i)
    {
        if (input[i] > max)
        {
            max = input[i];
        }
    }

    float sum_exp = 0.0;
    for (size_t i = 0; i < size; ++i)
    {
        output[i] = exp(input[i] - max);
        sum_exp += output[i];
    }

    for (size_t i = 0; i < size; ++i)
    {
        output[i] /= sum_exp;
    }
}

float randn()
{
    float u1 = (float)rand() / RAND_MAX; // so that it is between [0;1]
    float u2 = (float)rand() / RAND_MAX;
    float x = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return x;
}
