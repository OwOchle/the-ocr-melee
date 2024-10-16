#include "../../utils/array.h"
#include "../network.h"
#include "../training/backpropagation.h"
// #include "../../utils/matrix.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    uint16_t nodesPerLayers[] = {10, 2};

    Network *network = network_new(2, nodesPerLayers, 2);

    if (network == NULL)
    {
        errx(1, "Error creating network");
    }

    network_init_gaussian(network);
    network_print(network);

    float input[] = {4, 8};
    float output[] = {2, 4};

    GradiantData *grad = backprop(network, input, output);

    network_free(network);
    if (grad == NULL)
    {
        fprintf(stderr, "An error occured during backprop\n");
        return EXIT_FAILURE;
    }
    printf("gradiant layers count : %i\n", grad->layerCount);
    printf("gradiant bias of layer 0 : \n");
    array_print(grad->layers[0]->nodeCount, grad->layers[0]->bias);
    printf("gradiant bias of layer 1 : \n");
    array_print(grad->layers[1]->nodeCount, grad->layers[1]->bias);
    // gradiant_free(grad);

    return EXIT_SUCCESS;
}
