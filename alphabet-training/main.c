#include <err.h>
#include <stdlib.h>
#include <stdint.h>

#include "batch_conversion.h"
#include "read_image.h"
#include "../utils/matrix.h"
#include "../utils/verbose.h"
#include "../network/training/stochastic_gradient_descent.h"

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

    int res = stochastic_gradiant_descent(network, batch, 200, 32, 0.05f, 0.0f, NULL);

    network_free(network);
    batch_free(batch);
    free(output);

    return 0;
}
