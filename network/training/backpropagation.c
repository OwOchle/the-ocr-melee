#include "backpropagation.h"
// #include "../utils/activation_functions.h"
// #include "../utils/matrix.h"
// #include "../utils/array.h"
#include "../network.h"

#include <stdint.h>
#include <stdlib.h>

GradiantData *backprop(
    Network *network, size_t size, const float training_input[],
    const float desired_outputs[]
)
{
    GradiantData *gradiant = malloc(sizeof(GradiantData));
    if (network == NULL)
        return NULL;

    for (size_t i = 0; i < network->layerCount; i++)
    {
    }

    return NULL;
}
