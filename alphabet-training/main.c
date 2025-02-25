#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../network/evaluate.h"
#include "../network/file_io.h"
#include "../network/network.h"
#include "../network/training/stochastic_gradient_descent.h"
#include "../utils/matrix.h"
#include "../utils/progress.h"
// #include "../utils/array.h"
// #include "../utils/threaded_matrix.h"
// #include "../utils/verbose.h"
#include "batch_conversion.h"
#include "read_image.h"

#define THREAD_COUNT 1

// Hyper parameters
#define ETA 0.075f
#define LAMBDA 0.001f
#define HIDDEN_LAYER_COUNT1 128
#define MINI_BATCH_SIZE 32

Network *get_network(char *path)
{
    Network *net = NULL;
    NETWORK_ERRNO err = network_read(&net, path);

    if (err != NO_ERROR)
    {
        fprintf(
            stderr, "\e[1;33mResult file empty. Creating new "
                    "one. \e[0m\n"
        );
        uint16_t layers[] = {HIDDEN_LAYER_COUNT1, 26};

        net = network_new(2, layers, IMAGE_SIZE * IMAGE_SIZE);

        // network_init_flat(net);
        network_init_gaussian(net);
    }

    return net;
}

void print_letter(float image[], size_t size) {
    for (size_t y = 0; y < size; y++) {
        for (size_t x = 0; x < size; x++) {
            printf("%0.f", image[y * size + x]);
        }
        printf("\n");
    }
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

    pb_init(epochs, 0);
    // mat_th_init_threadpool(THREAD_COUNT);

    size_t count;

    InputImage *output = load_directory(argv[1], &count);

    if (output == NULL)
    {
        errx(2, "output is null");
    }

    int index = 0;
    // printf(
    //     "name: %s, character: %c, category: %d\n", output[index].name,
    //     output[index].character, output[index].category
    // );
    verbose_print_matrix_uchar(output[index].image, IMAGE_SIZE, IMAGE_SIZE);

    Batch *batch = images_to_batch(count, output);


    // for (size_t i = 100; i < 110; i++)
    // {
    //     print_letter(batch->layers[i]->inputData, IMAGE_SIZE);
    // }

    
    // array_print(IMAGE_SIZE * IMAGE_SIZE, batch->layers[0]->inputData);

    printf(
        "\n\e[1;34mTrainingData infos:\e[0m\n  - image size      = %ux%u\n  - data set size   = %zu\n", IMAGE_SIZE,
        IMAGE_SIZE, count
    );
    printf(
        "\e[1;34mHyperparameters:\e[0m\n  - mini-batch size = %i\n  - eta             = %.3f\n  - lambda          = %.3f\n",
        MINI_BATCH_SIZE, ETA, LAMBDA
    );

    sleep(1);

    printf("\n\e[1;35mRunning with %u thread.\e[0m\n", THREAD_COUNT);

    if (batch == NULL)
    {
        errx(3, "batch is null");
    }

    Network *network = get_network(argv[2]);

    if (network == NULL)
    {
        errx(4, "network is null (main.c:%d)", __LINE__);
    }

    sleep(1);

    printf("\e[1;37m\n========= Running %zu epochs ========\e[0m\n", epochs);

    printf("\n________________START_______________\n");
    printf(
        "Accuracy on training data: %i / %i\n", accuracy(network, batch),
        batch->batchSize
    );
    printf(
        "Cost on training data: %f\n____________________________________\n\n",
        total_cost(network, batch, 0.0f)
    );

    sleep(1);

    pb_start();

    int res = stochastic_gradiant_descent(
        network, batch, epochs, MINI_BATCH_SIZE, ETA, LAMBDA, NULL
    );

    pb_destroy();

printf("\n\n_______________RESULT_______________\n");
    printf(
        "Accuracy on training data: %i / %i\n", accuracy(network, batch),
        batch->batchSize
    );
    printf(
        "Cost on training data: %f\n____________________________________\n\n",
        total_cost(network, batch, 0.0f)
    );

    NETWORK_ERRNO err = network_write(network, argv[2]);

    if (err != NO_ERROR)
    {
        fprintf(
            stderr,
            "\e[1;33m /!\\ error while reading network. error code: %d "
            "/!\\\e[0m\n",
            err
        );
    }

    // mat_th_destroy_threadpool();
    network_free(network);
    batch_free(batch);
    free(output);

    return 0;
}
