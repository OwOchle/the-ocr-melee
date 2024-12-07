#include "../file_io.h"
#include "../network.h"
#include "../../utils/array.h"

#include <err.h>
// #include <stdio.h>
#include <stdlib.h>

int main()
{
    Network *network;
    NETWORK_ERRNO error = network_read(&network, "netX.neuron");

    if (error != NO_ERROR) {
        errx(1, "Network read failed with error %d", error);
    }

    array_print(network->layers[1]->nodeCount, network->layers[1]->bias);

    float *input = calloc(2, sizeof(float));

    input[0] = 1.0f;
    input[1] = 0.0f;

    float *out = network_apply(network, input);

    array_print(network_last_layer_count(network), out);
    network_free(network);
    free(out);

    return EXIT_SUCCESS;
}