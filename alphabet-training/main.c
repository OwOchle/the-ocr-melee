#include <err.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../network/evaluate.h"
#include "../network/training/stochastic_gradient_descent.h"
#include "../utils/matrix.h"
#include "../utils/verbose.h"
#include "batch_conversion.h"
#include "read_image.h"
#include "utils/threaded_matrix.h"
#include "network/file_io.h"
#include "network/network.h"

Network *get_network(char *path)
{
    Network *net = NULL;
    NETWORK_ERRNO err = network_read(&net, path);

    if (err != NO_ERROR)
    {
        fprintf(stderr, "\e[1;33m/!\\ error while reading network. Creating new one /!\\\e[0m\n");
        uint16_t layers[] = { 60, 26 };

        net = network_new(2, layers, IMAGE_SIZE * IMAGE_SIZE);

        network_init_gaussian(net);
    }

    return net;
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        errx(1, "usage: ./training <image_path> <network_path> <epoch_count>");
    }

    char *end;
    size_t epochs = strtoul(argv[3], &end, 10);

    if (*end != '\0')
    {
        errx(1, "invalid epoch count: %s", argv[3]);
    }

    mat_th_init_threadpool(2);

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

    Network *network = get_network(argv[2]);

    if (network == NULL)
    {
        errx(4, "network is null (main.c:%d)", __LINE__);
    }

    printf("========= Running %u epochs ========\n", epochs);

    printf("\n_________START________\n\n");
    printf(
        "Accuracy on training data: %i / %i\n", accuracy(network, batch),
        batch->batchSize
    );
    printf(
        "Cost on training data: %f\n________________________\n",
        total_cost(network, batch, 0.0f)
    );

    int res = stochastic_gradiant_descent(
        network, batch, epochs, 32, 0.01f, 0.001f, NULL
    );

    printf("\n_________RESULT________\n\n");
    printf(
        "Accuracy on training data: %i / %i\n", accuracy(network, batch),
        batch->batchSize
    );
    printf(
        "Cost on training data: %f\n________________________\n",
        total_cost(network, batch, 0.0f)
    );

    NETWORK_ERRNO err = network_write(network, argv[2]);

    if (err != NO_ERROR)
    {
        fprintf(stderr, "\e[1;33m /!\\ error while reading network. error code: %d /!\\\e[0m\n", err);
    }

    mat_th_destroy_threadpool();
    network_free(network);
    batch_free(batch);
    free(output);

    return 0;
}
