#include "backpropagation.h"

#include "../../utils/array.h"
#include "../../utils/matrix.h"
#include "../network.h"
#include "../network_utils/activation_functions.h"
#include "../network_utils/cost_functions.h"

#include <stdint.h>
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

    // Init Gradiant data (by passing values from the network)

    GradiantData *gradiant = malloc(sizeof(GradiantData));
    if (gradiant == NULL)
        return NULL;

    gradiant->entryCount = network->entryCount;
    gradiant->layerCount = layerCount;
    gradiant->layers = calloc(layerCount, sizeof(Layer *));

    if (network->layers == NULL)
        return NULL;

    for (char i = 0; i < layerCount; i++)
    {
        const int nodeCount = network->layers[i]->nodeCount;
        GradiantLayer *layer = malloc(sizeof(GradiantLayer));

        if (layer == NULL)
            return NULL;

        gradiant->layers[i] = layer;

        layer->nodeCount = nodeCount;

        layer->weights = calloc(network->layers[i]->nodeCount, sizeof(float));
        if (layer->weights == NULL)
            return NULL;

        layer->bias = calloc(nodeCount, sizeof(float));
        if (layer->bias == NULL)
            return NULL;
    }

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
    float **activation_matrix = create_layered_matrix(network);
    if (activation_matrix == NULL)
    {
        return NULL;
    }

    // Used to store intermediary values before the activation
    float **z_matrix = create_layered_matrix(network);
    if (z_matrix == NULL)
    {
        return NULL;
    }

    for (size_t x = 0; x < layerCount; x++)
    {
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
        Vector vector_z = matrix_multiply_array(
            pastNodeCount, nodeCount, weights, input_size, activation
        );
        if (vector_z == NULL)
        {
            return NULL;
        }
        matrix_add_array(pastNodeCount, 1, vector_z, bias);

        // Storing the z vector to use later to calculate the delta
        z_matrix[x] = vector_z;

        // Creating the next activation vector : sigmoid(vector_z)
        free(activation);
        activation = calloc(nodeCount, sizeof(float));
        if (activation == NULL)
        {
            return NULL;
        }
        sigmoid(
            nodeCount, vector_z, activation
        ); // TODO: we need to use softmax for the output layer

        // Storing the activation to use later in the backward pass
        activation_matrix[x] = activation;
    }

    // backward pass

    const uint16_t pastLayerCount = network->layers[layerCount - 2]->nodeCount;

    /* The size of delta is the number of nodes in the output layer.
     * Because delta represents the error in the output layer :
     * (activation - training_output) for Cross Entropy,
     * which is then used to propagate the error backward through the network.
     * THAT'S WHY I THINK IT'S CALLED BACK PROPAGATION!
     */
    Vector delta = calloc(output_size, sizeof(float));
    if (delta == NULL)
    {
        return NULL;
    }
    float *lastActivation = activation_matrix[layerCount - 1];
    cross_entropy_delta(output_size, lastActivation, desired_outputs, delta); //

    // Saving the delta as the last bias layer of the gradiant
    free(gradiant->layers[layerCount - 1]->bias);
    gradiant->layers[layerCount - 1]->bias = delta;

    // Saving (delta * activations[layerCount-2]) as the last weight layer of
    // the gradiant
    for (size_t y = 0; y < output_size; y++)
    {
        for (size_t x = 0; x < pastLayerCount; x++)
        {
            float *ptr = array_get_as_matrix_ptr(
                gradiant->layers[layerCount - 1]->weights, pastLayerCount, x, y
            );

            *ptr = delta[y] * activation_matrix[layerCount - 2][x];
        }
    }

    // Warning: Math ahead, I won't be of any help

    // backward pass so we are doing layers backwards
    for (size_t i = layerCount - 2; i > 0; i--)
    {
        const Layer *layer = network->layers[i];
        const Layer *nextLayer = network->layers[i + 1];
        const uint16_t nodeCount = layer->nodeCount;
        const uint16_t nextNodeCount = nextLayer->nodeCount;

        const Matrix nextWeights = nextLayer->weights;

        Vector z = z_matrix[i];

        Vector sp = calloc(nodeCount, sizeof(float));
        if (sp == NULL)
        {
            return NULL;
        }
        sigmoid_prime(nodeCount, z, sp);

        // delta = (weights[i + 1].transposed() * delta) * sp
        Vector newDelta = calloc(nextNodeCount, sizeof(float));
        Matrix transposedNextWeights =
            calloc(nextNodeCount * nodeCount, sizeof(float));
        matrix_transpose(
            nextNodeCount, nodeCount, nextWeights, transposedNextWeights
        );
        newDelta = matrix_multiply_array(
            nextNodeCount, nodeCount, transposedNextWeights, nodeCount, delta
        );
        for (size_t j = 0; j < nodeCount; j++)
        {
            newDelta[j] = newDelta[j] * sp[j];
        }

        free(sp);
        free(transposedNextWeights);

        free(delta);
        delta = newDelta;

        // Saving to gradiant bias
        free(gradiant->layers[i]->bias);
        gradiant->layers[i]->bias = delta;

        // Saving to gradiant weights
        for (size_t j = 0; j < nodeCount; j++)
        {
            float *ptr = array_get_as_matrix_ptr(
                gradiant->layers[i]->weights, nodeCount, j, 0
            );
            *ptr = delta[j] * activation_matrix[i - 1][j];
        }
    }

    free(activation);
    free_layered_matrix(activation_matrix, network);
    free_layered_matrix(z_matrix, network);
    free(delta);

    return gradiant;
}

void gradiant_free(GradiantData *gradiant)
{
    for (char l = 0; l < gradiant->layerCount; l++)
    {
        GradiantLayer *layer = gradiant->layers[l];

        free(layer->bias);
        free(layer->weights);
        free(layer);
    }

    free(gradiant->layers);
    free(gradiant);
}

float **create_layered_matrix(const Network *network)
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

void free_layered_matrix(float **layered_matrix, const Network *network)
{
    size_t layerCount = network->layerCount;
    for (size_t i = 0; i < layerCount; i++)
    {
        free(layered_matrix[i]);
    }
    free(layered_matrix);
}
