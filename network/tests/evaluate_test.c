// #include "../../utils/array.h"
#include "../network.h"
// #include "../training/backpropagation.h"
// #include "../../utils/matrix.h"
#include "../evaluate.h"
#include "../../alphabet-training/batch_conversion.h"

#include <complex.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
        if (argc < 2)
    {
        errx(1, "usage: ./evaluate_test <image_path>");
    }

    uint16_t nodesPerLayers[] = {30, 10};

    Network *network = network_new(2, nodesPerLayers, IMAGE_SIZE * IMAGE_SIZE);

    if (network == NULL)
    {
        errx(1, "Error creating network");
    }

    network_init_gaussian(network);
    network_print(network);

    size_t count;
    InputImage *output = load_directory(argv[1], &count);

    Batch *batch = images_to_batch(count, output);
    printf("Accuracy : %d\n", accuracy(network, batch));
    printf("Total initial cost : %f\n", total_cost(network, batch, 0.0f));

    free(output);
    batch_free(batch);
    batch = NULL;
    network_free(network);

    return EXIT_SUCCESS;
}