#include "network.h"

#include <err.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    uint16_t nodesPerLayers[] = {1024, 512, 26};

    Network *network = network_new(3, nodesPerLayers, 625);

    if (network == NULL)
    {
        errx(1, "Could not allocate a new network");
    }

    init_flat_network(network);

    free_network(network);

    return EXIT_SUCCESS;
}