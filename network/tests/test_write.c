#include "../network.h"
#include "../file_io.h"
#include "../../utils/array.h"
#include "err.h"
#include <stdlib.h>


int main(int argc, char **argv) {
    uint16_t nodesPerLayers[] = {16, 8};

    Network *network = network_new(2, nodesPerLayers, 2);

    if (network == NULL) {
        errx(1, "Error creating network");
    }

    network_init_gaussian(network);

    NETWORK_ERRNO error = network_write(network, "netX.neuron");

    if (error != NO_ERROR) {
        network_free(network);
        errx(1, "Network writing failed with error %d", error);
    }

    array_float_print(network->layers[1]->nodeCount, network->layers[1]->bias);

    network_free(network);

    return EXIT_SUCCESS;
}