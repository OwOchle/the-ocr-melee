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
    const Network *network, const size_t input_size,
    const float training_input[], const size_t output_size,
    const float desired_outputs[]
)
{
    // Init Gradiant data

    GradiantData *gradiant = malloc(sizeof(GradiantData));
    if (gradiant == NULL)
        return NULL;

    const char layerCount = network->layerCount;

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

    Vector activation = calloc(input_size, sizeof(float));
    for (size_t i = 0; i < input_size; i++)
    {
        activation[i] = training_input[i];
    }

    Matrix activations = calloc(input_size * layerCount, sizeof(float));
    for (size_t i = 0; i < input_size; i++)
    {
        Matrix ptr = array_get_as_matrix_ptr(activations, input_size, i, 0);
        *ptr = training_input[0];
    }

    float **z_values = create_z_matrix(network);

    for (size_t x = 0; x < layerCount; x++)
    {
        const Layer *layer = network->layers[x];
        const Vector bias = layer->bias;
        Matrix weights = layer->weights;
        const int nodeCount = layer->nodeCount;

        int pastLayerCount;
        if (x == 0)
        {
            pastLayerCount = network->entryCount;
        }
        else
        {
            pastLayerCount = network->layers[x - 1]->nodeCount;
        }

        Vector vector_z = matrix_multiply_array(
            pastLayerCount, nodeCount, weights, input_size, activation
        );

        matrix_add_array(
            pastLayerCount, 1, vector_z, bias
        ); // could also do a simple loop since z is a vector

        z_values[x] = vector_z;

        free(activation);
        activation = calloc(nodeCount, sizeof(float));
        sigmoid(nodeCount, vector_z, activation);

        for (size_t y = 0; y < nodeCount; y++)
        {
            Matrix ptr = array_get_as_matrix_ptr(activations, input_size, x, y);
            *ptr = activation[y];
        }
    }

    // backward pass

    const uint16_t outputNodeCount = network->layers[layerCount - 1]->nodeCount;
    const uint16_t pastLayerCount = network->layers[layerCount - 2]->nodeCount;

    /* The size of delta is the number of nodes in the output layer.
     * Because delta represents the error in the output layer,
     * which is used to propagate the error backward through the network. */
    Vector delta = calloc(outputNodeCount, sizeof(float));

    const Vector lastActivation =
        array_get_as_matrix_ptr(activations, input_size, 0, layerCount - 1);

    cross_entropy_delta(
        outputNodeCount, lastActivation, desired_outputs, delta
    );

    gradiant->layers[layerCount - 1]->bias = delta;

    const Vector beforeLastActivation =
        array_get_as_matrix_ptr(activations, input_size, 0, layerCount - 2);

    for (size_t i = 0; i < outputNodeCount; i++)
    {
        Matrix ptr = array_get_as_matrix_ptr(
            gradiant->layers[layerCount - 1]->weights, outputNodeCount, i, 0
        );
        *ptr = delta[i] * beforeLastActivation[i];
    }

    for (size_t y = 0; y < outputNodeCount; y++)
    {
        for (size_t x = 0; x < pastLayerCount; x++)
        {
            Matrix ptr = array_get_as_matrix_ptr(
                gradiant->layers[layerCount - 1]->weights, pastLayerCount, x, y
            );

            *ptr = delta[y] * beforeLastActivation[x];
        }
    }

    // Warning: Math ahead
    for (size_t i = layerCount - 2; i > 0; i--)
    {
        const Layer *layer = network->layers[i];
        const Layer *nextLayer = network->layers[i + 1];
        const uint16_t nodeCount = layer->nodeCount;
        const uint16_t nextNodeCount = nextLayer->nodeCount;

        const Matrix nextWeights = nextLayer->weights;

        Vector z = z_values[i];

        Vector sigma_prime = calloc(nodeCount, sizeof(float));
        sigmoid_prime(nodeCount, z, sigma_prime);

        Vector nextDelta = calloc(nextNodeCount, sizeof(float));
        Matrix transposedNextWeights =
            calloc(nextNodeCount * nodeCount, sizeof(float));
        matrix_transpose(
            nextNodeCount, nodeCount, nextWeights, transposedNextWeights
        );

        nextDelta = matrix_multiply_array(
            nextNodeCount, nodeCount, transposedNextWeights, nodeCount, delta
        );
        for (size_t j = 0; j < nodeCount; j++)
        {
            delta[j] = nextDelta[j] * sigma_prime[j];
        }

        free(sigma_prime);
        free(transposedNextWeights);

        free(delta);
        delta = nextDelta;

        gradiant->layers[i]->bias = delta;

        Vector beforeActivation =
            array_get_as_matrix_ptr(activations, input_size, 0, i - 1);
        for (size_t j = 0; j < nodeCount; j++)
        {
            Matrix ptr = array_get_as_matrix_ptr(
                gradiant->layers[i]->weights, nodeCount, j, 0
            );
            *ptr = delta[j] * beforeActivation[j];
        }
    }

    free(activation);
    free(activations);
    free(z_values);
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

float **create_z_matrix(const Network *network)
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

void free_z_matrix(float **z_matrix, const Network *network)
{
    size_t layerCount = network->layerCount;
    for (size_t i = 0; i < layerCount; i++)
    {
        free(z_matrix[i]);
    }
    free(z_matrix);
}
