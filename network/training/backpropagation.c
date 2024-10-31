#include "backpropagation.h"

#include "../../utils/matrix.h"
#include "../network.h"
#include "../network_utils/activation_functions.h"
#include "../network_utils/cost_functions.h"

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

    printf("Beginning alloc gradiant.\n");

    GradiantData *gradiant = gradiant_new(network);
    if (gradiant == NULL)
    {
        return NULL;
    }

    printf("  - Allocation of gradiant successfull\n\nBeginning feedforward.\n"
    );

    // feedforward

    // Contain the activation function, sigmoid in our case.
    Vector activation = calloc(input_size, sizeof(float));
    if (activation == NULL)
    {
        return NULL;
    }
    for (size_t i = 0; i < input_size; i++)
    {
        activation[i] = training_input[i];
    }

    // Used to store the netwoks values after applying the activation function
    float **activation_matrix = alloc_activation_matrix(network);
    if (activation_matrix == NULL)
    {
        return NULL;
    }

    free(activation_matrix[0]);
    activation_matrix[0] = activation;

    // Used to store intermediary values before the activation
    float **z_matrix = alloc_z_matrix(network);
    if (z_matrix == NULL)
    {
        return NULL;
    }

    printf("  - Allocation of matrixes successfull.\n");

    for (size_t x = 0; x < layerCount; x++)
    {
        printf("  - Beginning layer %zu.\n", x);

        Layer *layer = network->layers[x];

        Vector bias = layer->bias;
        Matrix weights = layer->weights;

        int nodeCount = layer->nodeCount;

        int pastNodeCount;
        if (x == 0)
        {
            pastNodeCount = network->entryCount;
        }
        else
        {
            pastNodeCount = network->layers[x - 1]->nodeCount;
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
        free(z_matrix[x]);
        z_matrix[x] = z_vector;

        printf("    - Created a z vector.\n");

        // Creating the next activation vector : sigmoid(vector_z)
        activation = calloc(nodeCount, sizeof(float));
        if (activation == NULL)
        {
            printf("  ~ Error while allocating new activation.\n");
            return NULL;
        }
        sigmoid(
            nodeCount, z_vector, activation
        ); // TODO: we need to use softmax for the output layer

        // Storing the activation to use later in the backward pass
        free(activation_matrix[x + 1]);
        activation_matrix[x + 1] = activation;

        printf("    - Created an activation vector.\n");
    }

    printf("  - Feedforward successfull!\n\nBeginning backward pass!\n");

    // backward pass

    printf("  - Beginning the output layer with node count %u\n", output_size);

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
    float *lastActivation = activation_matrix[layerCount - 1];
    cross_entropy_delta(output_size, lastActivation, desired_outputs, delta); //

    printf("    - Created the output delta vector.\n");

    // Saving the delta as the last bias layer of the gradiant
    free(gradiant->layers[layerCount - 1]->bias);
    gradiant->layers[layerCount - 1]->bias = delta;

    printf("    - Saved the output delta vector to gradiant bias.\n");

    // Saving (delta * activations[-1]) as the last weight layer of
    // the gradiant
    Matrix activated_delta = matrix_multiply(
        1, output_size, delta, output_size, 1, activation_matrix[layerCount - 2]
    );
    if (activated_delta == NULL)
    {
        printf("    ~ Error while mult the activated delta.\n");
        return NULL;
    }
    free(gradiant->layers[layerCount - 1]->weights);
    gradiant->layers[layerCount - 1]->weights = activated_delta;

    printf("    - Saved the output delta vector to gradiant weights.\n");

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

        printf("  - Beginning layer %hhi with node count %u\n", l, nodeCount);

        Vector z = z_matrix[l];

        // array_print(nodeCount, z);
        Vector sp = calloc(nodeCount, sizeof(float));
        if (sp == NULL)
        {
            printf("    ~ Error while alloc sigmoid prime.\n");
            return NULL;
        }
        sigmoid_prime(nodeCount, z, sp);

        printf("    - Created sigmoid prime vector.\n");

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

        Vector newDelta = matrix_multiply(
            nextNodeCount, nodeCount, transposedNextWeights, 1, nextNodeCount,
            delta
        );
        if (newDelta == NULL)
        {
            printf("    ~ Error while mult delta vector.\n");
            return NULL;
        }
        for (size_t j = 0; j < nodeCount; j++)
        {
            newDelta[j] = newDelta[j] * sp[j];
        }

        printf("    - Successfully created delta vector.\n");

        free(sp);
        free(transposedNextWeights);

        delta = newDelta;

        // Saving to gradiant bias
        free(gradiant->layers[l]->bias);
        gradiant->layers[l]->bias = delta;

        printf("    - Saved delta vector to gradiant bias.\n");

        // Saving to gradiant weights
        Matrix activated_delta = matrix_multiply(
            1, nodeCount, delta, pastNodeCount, 1, activation_matrix[l]
        );
        if (activated_delta == NULL)
        {
            printf("  ~ Error while mult the activated delta.\n");
            return NULL;
        }
        free(gradiant->layers[layerCount - 1]->weights);
        gradiant->layers[layerCount - 1]->weights = activated_delta;

        printf("    - Saved delta vector to gradiant weights.\n");
    }

    printf("  - Backward pass successfull!\n\nFreeing pointers...\n");

    free_activation_matrix(activation_matrix, network);
    free_z_matrix(z_matrix, network);

    printf("  - Freeing Pointers successfull!\n\nBack Prop done!\n");

    return gradiant;
}

float **alloc_z_matrix(const Network *network)
{
    size_t layerCount = network->layerCount;

    float **z_matrix = calloc(layerCount, sizeof(float *));
    if (z_matrix == NULL)
    {
        return NULL;
    }

    for (size_t i = 0; i < layerCount; i++)
    {
        z_matrix[i] = calloc(network->layers[i]->nodeCount, sizeof(float));
        if (z_matrix[i] == NULL)
        {
            return NULL;
        }
    }

    return z_matrix;
}

float **alloc_activation_matrix(const Network *network)
{
    size_t layerCount = network->layerCount;

    float **act_matrix = calloc(layerCount + 1, sizeof(float *));
    if (act_matrix == NULL)
    {
        return NULL;
    }

    act_matrix[0] = calloc(network->entryCount, sizeof(float));
    for (size_t i = 0; i < layerCount; i++)
    {
        act_matrix[i + 1] =
            calloc(network->layers[i]->nodeCount, sizeof(float));
        if (act_matrix[i + 1] == NULL)
        {
            return NULL;
        }
    }

    return act_matrix;
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
