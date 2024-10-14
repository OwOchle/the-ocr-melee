#include "../network.h"
// #include "../training/backpropagation.h"
// #include "../../utils/array.h"
// #include "../../utils/matrix.h"

#include <err.h>
#include <stdlib.h>

int main()
{
    uint16_t nodesPerLayers[] = {3, 2};

    Network *network = network_new(2, nodesPerLayers, 2);

    if (network == NULL)
    {
        errx(1, "Error creating network");
    }

    network_init_gaussian(network);
    network_print(network);

    network_free(network);

    return EXIT_SUCCESS;
}
