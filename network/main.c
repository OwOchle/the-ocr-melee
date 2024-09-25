#include "network.h"
#include <stdlib.h>
#include <err.h>

int main() {
    int16_t nodesPerLayers[] = {
        3, 4, 5
    };

    Network *network = network_new(3, nodesPerLayers, 2);

    if (network == NULL) {
        errx(1, "Could not allocate a new network");
    }

    init_flat_network(network);

    free_network(network);

    return EXIT_SUCCESS;
}