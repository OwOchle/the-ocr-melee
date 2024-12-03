#include "file_io.h"
#include "network.h"
#include "../utils/array.h"
// #include "../utils/matrix.h"

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

    network_init_gaussian(network);

    network_write(network, "net.neuron");

    Network *networkR;
    int error = network_read(&networkR, "net.neuron");

    if (networkR == NULL)
    {
        errx(
            1, "Could not read file with error : %d and errno %d", error, errno
        );
    }

    // network_write(networkR, "netR.neuron");

    float *input = calloc(2, sizeof(float));

    input[0] = 1.0f;
    input[1] = 0.0f;

    float *out = network_apply(network, input);

    if (out == NULL)
    {
        network_free(network);
        network_free(networkR);
        errx(1, "Error happend while applying the network");
    }

    array_float_print(2, out);

    network_free(network);
    network_free(networkR);

    Network *networkX;
    
    NETWORK_ERRNO err = network_read(&networkX, "netR.neuron");

    if (err != NO_ERROR) {
        errx(1, "Network reading failed with error %d", err);
    }

    out = network_apply(networkX, input);

    if (out == NULL)
    {
        network_free(networkX);
        errx(1, "Error happend while applying the network");
    }

    array_float_print(2, out);

    network_free(networkX);
    free(out);

    return EXIT_SUCCESS;
}