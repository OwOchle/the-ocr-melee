#include "file_io.h"
#include "network.h"

#include <err.h>
#include <errno.h>
#include <stdio.h>
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

    Network *networkR;
    int error = network_read(&networkR, "net.neuron");

    if (networkR == NULL)
    {
        errx(
            1, "Could not read file with error : %d and errno %d", error, errno
        );
    }

    network_write(networkR, "netR.neuron");

    network_free(network);
    network_free(networkR);

    return EXIT_SUCCESS;
}