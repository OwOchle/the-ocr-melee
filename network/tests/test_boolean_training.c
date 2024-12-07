// #include "../../utils/array.h"
// #include "../evaluate.h"
#include "../training/stochastic_gradient_descent.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

    uint16_t nodesPerLayers[] = {4, 2};

    Network *network = network_new(2, nodesPerLayers, 2);

    if (network == NULL)
    {
        errx(1, "Error creating network");
    }

    network_init_gaussian(network);
    network_print(network);

    Batch *batch = batch_new(4, 2, 2);
    if (batch == NULL)
    {
        errx(1, "Error creating mini batch");
    }

    batch->layers[0]->inputData[0] = 0;
    batch->layers[0]->inputData[1] = 0;
    batch->layers[0]->outputData[0] = 0;
    batch->layers[0]->outputData[1] = 1;

    batch->layers[1]->inputData[0] = 0;
    batch->layers[1]->inputData[1] = 1;
    batch->layers[1]->outputData[0] = 1;
    batch->layers[1]->outputData[1] = 0;

    batch->layers[2]->inputData[0] = 1;
    batch->layers[2]->inputData[1] = 0;
    batch->layers[2]->outputData[0] = 1;
    batch->layers[2]->outputData[1] = 0;

    batch->layers[3]->inputData[0] = 1;
    batch->layers[3]->inputData[1] = 1;
    batch->layers[3]->outputData[0] = 0;
    batch->layers[3]->outputData[1] = 1;

    printf("\nBatch creation success \n");

    int result =
        stochastic_gradiant_descent(network, batch, 500, 4, 1.0f, 0, NULL);
    if (!result)
    {
        errx(1, "Error training the network");
    }

    // printf("\n\n______________________TESTS____________________\n");

    /* float *prediction = NULL;
    float *test = calloc(2, sizeof(float));
    test[0] = 1;
    test[1] = 0;

    prediction = network_apply(network, test);
    if (!result)
    {
        errx(1, "Error evaluating the network");
    }

    array_print(1, prediction); */

    network_free(network);
    batch_free(batch);
    /* free(test);
    free(prediction); */

    return EXIT_SUCCESS;
}
