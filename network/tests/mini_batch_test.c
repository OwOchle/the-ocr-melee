// #include "../../utils/array.h"
#include "../network.h"
#include "../training/update_mini_batch.h"
// #include "../../utils/matrix.h"

#include <err.h>
#include <stdio.h>
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

    Batch *mini_batch = batch_new(2, 2, 2);
    if (mini_batch == NULL)
    {
        errx(1, "Error creating mini batch");
    }

    printf(
        "bias[0], premier layer interne = %f\n\n", network->layers[0]->bias[0]
    );

    mini_batch->layers[0]->inputData[0] = 1;
    mini_batch->layers[0]->inputData[1] = 1;
    mini_batch->layers[0]->outputData[0] = 2;
    mini_batch->layers[0]->outputData[1] = 2;

    mini_batch->layers[1]->inputData[0] = 2;
    mini_batch->layers[1]->inputData[1] = 2;
    mini_batch->layers[1]->outputData[0] = 4;
    mini_batch->layers[1]->outputData[1] = 4;

    int res = update_mini_batch(network, mini_batch, 0.5, 0, 2);
    if (!res)
    {
        errx(1, "Error updating mini batch");
    }

    printf("\nUpdate mini batch success !!!!!\n");

    printf(
        "\nbias[0], premier layer interne = %f\n", network->layers[0]->bias[0]
    );
    network_free(network);
    batch_free(mini_batch);

    return EXIT_SUCCESS;
}
