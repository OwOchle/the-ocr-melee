#include <err.h>
#include <stdint.h>
#include <stdlib.h>

#include "../network/evaluate.h"
#include "../network/training/stochastic_gradient_descent.h"
#include "../utils/matrix.h"
#include "../utils/verbose.h"
#include "batch_conversion.h"
#include "read_image.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        errx(1, "Usage: ./training <image_path>");
    }

    size_t count;

    InputImage *output = load_directory(argv[1], &count);

    if (output == NULL)
    {
        errx(2, "output is null");
    }

    int index = 125;
    verbose_printf(
        "name: %s, character: %c, category: %d\n", output[index].name,
        output[index].character, output[index].category
    );
    verbose_print_matrix_uchar(output[index].image, IMAGE_SIZE, IMAGE_SIZE);

    Batch *batch = images_to_batch(count, output);

    if (batch == NULL)
    {
        errx(3, "batch is null");
    }

    uint16_t layers[] = {60, 26};

    Network *network = network_new(2, layers, IMAGE_SIZE * IMAGE_SIZE);

    network_init_gaussian(network);


    verbose_printf("\n_________START________\n\n");
    verbose_printf(
        "Accuracy on training data: %i / %i\n", accuracy(network, batch),
        batch->batchSize
    );
    verbose_printf(
        "Cost on training data: %f\n________________________\n",
        total_cost(network, batch, 0.0f)
    );

    int res = stochastic_gradiant_descent(
        network, batch, 750, 32, 0.01f, 0.001f, NULL
    );

    verbose_printf("\n_________RESULT________\n\n");
    verbose_printf(
        "Accuracy on training data: %i / %i\n", accuracy(network, batch),
        batch->batchSize
    );
    verbose_printf(
        "Cost on training data: %f\n________________________\n",
        total_cost(network, batch, 0.0f)
    );

    network_free(network);
    batch_free(batch);
    free(output);

    return 0;
}
