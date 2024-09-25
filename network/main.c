#include "network.h"
#include "file_io.h"

#include <err.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    uint16_t nodesPerLayers[] = {16, 8};

    Network *network = network_new(2, nodesPerLayers, 2);

    if (network == NULL)
    {
        errx(1, "Could not allocate a new network");
    }

    network_init_flat(network);

    network_write(network, "net.neuron");

    network_free(network);

    return EXIT_SUCCESS;
}