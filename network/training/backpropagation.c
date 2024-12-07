#include "backpropagation.h"

// #include "../../utils/array.h"
#include "../../utils/matrix.h"
#include "../network.h"
#include "../network_utils/activation_functions.h"
#include "../network_utils/cost_functions.h"
// #include "../../utils/threaded_matrix.h"
#include "../../utils/verbose.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef float *Vector;
typedef float *Matrix;

GradiantData *backprop(
    const Network *network, const float training_input[],
    const float desired_outputs[]
)
{
    const char layerCount = network->layerCount;
    const uint16_t input_size = network->entryCount;
    const uint16_t output_size = network->layers[layerCount - 1]->nodeCount;

    GradiantData *gradiant = gradiant_new(network);
    if (gradiant == NULL)
    {
        return NULL;
    }

    verbose_printf(
        "  - Allocation of gradiant successfull\n\nBeginning feedforward.\n"
    );

    // feedforward

    Vector activation = calloc(input_size, sizeof(float));
    if (activation == NULL)
        return NULL;

    for (size_t i = 0; i < input_size; i++)
    {
        activation[i] = training_input[i];
    }

    // if (extra == 0)
    //     array_print(input_size, activation);

    // Used to store the netwoks values after applying the activation function
    float **activation_matrix = calloc(layerCount + 1, sizeof(float *));
    if (activation_matrix == NULL)
        return NULL;

    activation_matrix[0] = activation;

    // Used to store intermediary values before the activation
    float **z_matrix = calloc(layerCount, sizeof(float *));
    if (z_matrix == NULL)
        return NULL;

    for (size_t l = 0; l < layerCount; l++)
    {
        Layer *layer = network->layers[l];

        Vector bias = layer->bias;
        Matrix weights = layer->weights;

        uint16_t nodeCount = layer->nodeCount;

        uint16_t pastNodeCount;
        if (l == 0)
        {
            pastNodeCount = network->entryCount;
        }
        else
        {
            pastNodeCount = network->layers[l - 1]->nodeCount;
        }

        // Creating a z vector which is : weight[layer] * activation +
        // bias[layer]
        Vector z_vector = matrix_multiply(
            pastNodeCount, nodeCount, weights, 1, pastNodeCount, activation
        );
        if (z_vector == NULL)
        {
            printf("  ~ Error while mult matrix.\n");
            return NULL;
        }
        matrix_add(1, nodeCount, z_vector, 1, nodeCount, bias);

        // Storing the z vector to use later to calculate the delta
        z_matrix[l] = z_vector;

        // Creating the next activation vector : sigmoid(vector_z)
        activation = calloc(nodeCount, sizeof(float));
        if (activation == NULL)
        {
            printf("  ~ Error while allocating new activation.\n");
            return NULL;
        }

        if (l == layerCount - 1)
        {
            softmax(nodeCount, z_vector, activation);
        }
        else
        {
            sigmoid(nodeCount, z_vector, activation);
        }

        // Storing the activation to use later in the backward pass
        activation_matrix[l + 1] = activation;
    }

    // backward pass

    verbose_printf(
        "  - Beginning the output layer with node count %u\n", output_size
    );

    const uint16_t beforeOutputLayerCount =
        network->layers[layerCount - 2]->nodeCount;

    /* The size of delta is the number of nodes in the output layer.
     * Because delta represents the error in the output layer :
     * (activation - training_output) for Cross Entropy,
     * which is then used to propagate the error backward through the network.
     * THAT'S WHY I THINK IT'S CALLED BACK PROPAGATION!
     */
    Vector delta = calloc(output_size, sizeof(float));
    if (delta == NULL)
    {
        printf("    ~ Error while allocating the output delta.\n");
        return NULL;
    }
    float *lastActivation = activation_matrix[layerCount]; // + 1 offset
    cross_entropy_delta(output_size, lastActivation, desired_outputs, delta);

    // array_float_print(output_size, delta);
    // array_float_print(100, activation_matrix[layerCount - 1]);

    // Saving the delta as the last bias layer of the gradiant
    free(gradiant->layers[layerCount - 1]->bias);
    gradiant->layers[layerCount - 1]->bias = delta;

    verbose_printf("    - Saved the output delta vector to gradiant bias.\n");

    // Saving (delta * activations[-1]) as the last weight layer of
    // the gradiant
    // Matrix activated_delta = matrix_multiply(
    //     1, beforeOutputLayerCount, activation_matrix[layerCount - 1],
    //     output_size, 1, delta
    // ); // + 1 offset
        Matrix activated_delta = matrix_multiply(
        1, output_size, delta, beforeOutputLayerCount, 1 , activation_matrix[layerCount - 1]
    ); // + 1 offset

    // array_float_print(beforeOutputLayerCount * output_size, activated_delta);


    // if (extra == 0)
    // {
    //     printf("%i\n", beforeOutputLayerCount * output_size);
    //     array_float_print(
    //         beforeOutputLayerCount * output_size, activated_delta
    //     );
    // }

    if (activated_delta == NULL)
    {
        printf("    ~ Error while mult the activated delta.\n");
        return NULL;
    }

    free(gradiant->layers[layerCount - 1]->weights);
    gradiant->layers[layerCount - 1]->weights = activated_delta;

    verbose_printf("    - Saved the output delta vector to gradiant weights.\n"
    );

    // Warning: Math ahead, I won't be of any help
    // backward pass so we are doing layers backwards
    for (char l = layerCount - 2; l >= 0; l--)
    {
        const Layer *layer = network->layers[l];
        const uint16_t nodeCount = layer->nodeCount;

        const Layer *nextLayer = network->layers[l + 1];
        const uint16_t nextNodeCount = nextLayer->nodeCount;
        const Matrix nextWeights = nextLayer->weights;

        uint16_t pastNodeCount;
        if (l == 0)
        {
            pastNodeCount = network->entryCount;
        }
        else
        {
            pastNodeCount = network->layers[l - 1]->nodeCount;
        }

        verbose_printf(
            "  - Beginning layer %hhi with node count %u\n", l, nodeCount
        );

        Vector z = z_matrix[l];

        // array_print(nodeCount, z);
        Vector sig_prime = calloc(nodeCount, sizeof(float));
        if (sig_prime == NULL)
        {
            printf("    ~ Error while alloc sigmoid prime.\n");
            return NULL;
        }
        sigmoid_prime(nodeCount, z, sig_prime);

        // delta = (weights[i + 1].transposed() * delta) * sp
        // Vector newDelta = calloc(nextNodeCount, sizeof(float));
        Matrix transposedNextWeights =
            calloc(nextNodeCount * nodeCount, sizeof(float));
        if (transposedNextWeights == NULL)
        {
            printf("    ~ Error while alloc transposed weights.\n");
            return NULL;
        }
        matrix_transpose(
            nextNodeCount, nodeCount, nextWeights, transposedNextWeights
        );

        Vector new_delta = matrix_multiply(
            nextNodeCount, nodeCount, transposedNextWeights, 1, nextNodeCount,
            delta
        );

        if (new_delta == NULL)
        {
            printf("    ~ Error while mult delta vector.\n");
            return NULL;
        }
        for (size_t j = 0; j < nodeCount; j++)
        {
            new_delta[j] = new_delta[j] * sig_prime[j];
        }

        // if (extra == 0)
        //     array_float_print(nodeCount, new_delta);

        free(sig_prime);
        free(transposedNextWeights);

        delta = new_delta;

        // Saving to gradiant bias
        free(gradiant->layers[l]->bias);
        gradiant->layers[l]->bias = delta;

        // Saving to gradiant weights
        /* Matrix activated_delta = matrix_multiply(
            1, nodeCount, delta, pastNodeCount, 1, activation_matrix[l]
        ); */
        Matrix activated_delta = matrix_multiply(
            1, pastNodeCount, activation_matrix[l], nodeCount, 1, delta
        );
        if (activated_delta == NULL)
        {
            printf("  ~ Error while mult the activated delta.\n");
            return NULL;
        }

        free(gradiant->layers[l]->weights);
        gradiant->layers[l]->weights = activated_delta;
    }

    free_activation_matrix(activation_matrix, network);
    free_z_matrix(z_matrix, network);

    return gradiant;
}

void free_z_matrix(float **z_matrix, const Network *network)
{
    size_t layerCount = network->layerCount;
    for (size_t i = 0; i < layerCount; i++)
    {
        free(z_matrix[i]);
    }
    free(z_matrix);
}

void free_activation_matrix(float **activation_matrix, const Network *network)
{
    size_t layerCount = network->layerCount;
    for (size_t i = 0; i < layerCount + 1; i++)
    {
        free(activation_matrix[i]);
    }
    free(activation_matrix);
}
