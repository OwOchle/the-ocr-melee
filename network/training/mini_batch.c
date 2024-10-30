#include "mini_batch.h"
#include "backpropagation.h"

#include "../../utils/array.h"
#include "../../utils/matrix.h"
#include "../network.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef float *Vector;
typedef float *Matrix;

int update_mini_batch(
    Network *network, MiniBatch *mini_batch, float eta, float lambda,
    int total_training_size
)
{
    const char layerCount = network->layerCount;
    const uint16_t input_size = network->entryCount;
    const uint16_t output_size = network->layers[layerCount - 1]->nodeCount;
    uint16_t batch_size = mini_batch->batchSize;

    printf("Beginning alloc gradiant.\n");

    GradiantData *gradiant = malloc(sizeof(GradiantData));
    if (gradiant == NULL)
    {
        return 0;
    }

    gradiant->entryCount = input_size;
    gradiant->layerCount = layerCount;
    gradiant->layers = calloc(layerCount, sizeof(GradiantLayer *));

    if (network->layers == NULL)
    {
        return 0;
    }

    for (char i = 0; i < layerCount; i++)
    {
        const int nodeCount = network->layers[i]->nodeCount;
        GradiantLayer *layer = malloc(sizeof(GradiantLayer));

        if (layer == NULL)
        {
            return 0;
        }

        gradiant->layers[i] = layer;

        layer->nodeCount = nodeCount;

        int pastLayerCount;
        if (i == 0)
        {
            pastLayerCount = input_size;
        }
        else
        {
            pastLayerCount = network->layers[i - 1]->nodeCount;
        }

        layer->weights = calloc(
            network->layers[i]->nodeCount * pastLayerCount, sizeof(float)
        );
        if (layer->weights == NULL)
        {
            return 0;
        }

        layer->bias = calloc(nodeCount, sizeof(float));
        if (layer->bias == NULL)
        {
            return 0;
        }
    }

    printf("  - Allocation of gradiant successfull\n\nBeginning Accumulation.\n"
    );
    for (size_t tupleIdx = 0; tupleIdx < batch_size; tupleIdx++)
    {
        printf("  - Beginning Summation of tuple[%zu]\n", tupleIdx);

        Vector trainingData = mini_batch->layers[tupleIdx]->inputData;
        Vector desiredData = mini_batch->layers[tupleIdx]->outputData;

        printf("  - Creating a gradiant using the backpropagation...\n");
        printf("____________________________________________________\n\n");

        // Create a new gradiant using a tuple from the mini batch
        GradiantData *deltaGradiant =
            backprop(network, trainingData, desiredData);
        printf("____________________________________________________\n\n");
        printf("  - Gradiant successfully created.\n");

        // nabla = nabla + gradiant
        for (size_t l = 0; l < layerCount; l++)
        {
            printf("    - Adding layer %zu\n", l);
            GradiantLayer *deltaGradiantLayer = deltaGradiant->layers[l];
            GradiantLayer *gradiantLayer = gradiant->layers[l];
            uint16_t nodeCount = gradiantLayer->nodeCount;

            uint16_t pastNodeCount;
            if (l == 0)
            {
                pastNodeCount = network->entryCount;
            }
            else
            {
                pastNodeCount = network->layers[l - 1]->nodeCount;
            }

            matrix_add(
                1, nodeCount, gradiantLayer->bias, 1, nodeCount,
                deltaGradiantLayer->bias
            );
            printf("      - nodeCount     = %hu\n", nodeCount);
            matrix_add(
                nodeCount, pastNodeCount, gradiantLayer->weights, nodeCount,
                pastNodeCount, deltaGradiantLayer->weights
            );
            printf("      - Added weights\n");
        }

        gradiant_free(deltaGradiant);
    }

    printf("\n  - Successfully finished the Accumulation.\n");

    printf("  - Beginning the training.\n");
    for (size_t l = 0; l < layerCount; l++)
    {
        printf("    - Training layer %zu\n", l);
        uint16_t nodeCount = network->layers[l]->nodeCount;
        float learningRate = (eta / mini_batch->batchSize);

        // Weights Update

        Matrix weightLayer = network->layers[l]->weights;
        Matrix gradiantWeightLayer = gradiant->layers[l]->weights;

        float weightDecayFactor = (1 - eta * (lambda / total_training_size));

        uint16_t pastNodeCount;
        if (l == 0)
        {
            pastNodeCount = network->entryCount;
        }
        else
        {
            pastNodeCount = network->layers[l - 1]->nodeCount;
        }

        for (size_t y = 0; y < nodeCount; y++)
        {
            for (size_t x = 0; x < pastNodeCount; x++)
            {
                float *weight_ptr =
                    array_get_as_matrix_ptr(weightLayer, nodeCount, y, x);
                float weight =
                    array_get_as_matrix(weightLayer, nodeCount, y, x);
                float gradiantWeight =
                    array_get_as_matrix(gradiantWeightLayer, nodeCount, y, x);

                *weight_ptr =
                    weightDecayFactor * weight - learningRate * gradiantWeight;
            }
        }

        printf("      - Trained the weights\n");

        // Bias Update

        Vector biasLayer = network->layers[l]->bias;
        Vector gradiantBiasLayer = gradiant->layers[l]->bias;

        for (size_t x = 0; x < nodeCount; x++)
        {
            float *bias = array_get_as_matrix_ptr(biasLayer, nodeCount, x, 0);
            float *gradiantBias =
                array_get_as_matrix_ptr(gradiantBiasLayer, nodeCount, x, 0);

            *bias = (*bias) - learningRate * (*gradiantBias);
        }
        printf("      - Trained the bias\n");
    }
    printf("  - Training done!\n");

    printf("  - Freeing gradiant...\n");
    gradiant_free(gradiant);

    return 1;
}

MiniBatch *
mini_batch_new(uint16_t batchSize, size_t input_size, size_t output_size)
{
    MiniBatch *mini_batch = malloc(sizeof(MiniBatch));
    if (!mini_batch)
        return NULL;

    mini_batch->batchSize = batchSize;
    mini_batch->layers = malloc(batchSize * sizeof(MiniBatchLayer *));
    if (!mini_batch->layers)
    {
        free(mini_batch);
        return NULL;
    }

    for (size_t i = 0; i < batchSize; i++)
    {
        mini_batch->layers[i] = malloc(sizeof(MiniBatchLayer));
        if (!mini_batch->layers[i])
        {
            // Libérer les couches déjà alloué si probs
            for (size_t j = 0; j < i; j++)
            {
                free(mini_batch->layers[j]->inputData);
                free(mini_batch->layers[j]->outputData);
                free(mini_batch->layers[j]);
            }
            free(mini_batch->layers);
            free(mini_batch);
            return NULL;
        }

        mini_batch->layers[i]->inputData = malloc(input_size * sizeof(float));
        mini_batch->layers[i]->outputData = malloc(output_size * sizeof(float));
        if (!mini_batch->layers[i]->inputData ||
            !mini_batch->layers[i]->outputData)
        {
            // Encore, libérer en cas de malloc qui rate
            free(mini_batch->layers[i]->inputData);
            free(mini_batch->layers[i]->outputData);
            free(mini_batch->layers[i]);
            for (size_t j = 0; j < i; j++)
            {
                free(mini_batch->layers[j]->inputData);
                free(mini_batch->layers[j]->outputData);
                free(mini_batch->layers[j]);
            }
            free(mini_batch->layers);
            free(mini_batch);
            return NULL;
        }
    }
    return mini_batch;
}

void mini_batch_free(MiniBatch *mini_batch)
{
    if (!mini_batch)
        return;

    for (size_t i = 0; i < mini_batch->batchSize; i++)
    {
        if (mini_batch->layers[i])
        {
            free(mini_batch->layers[i]->inputData);
            free(mini_batch->layers[i]->outputData);
            free(mini_batch->layers[i]);
        }
    }
    free(mini_batch->layers);
    free(mini_batch);
}
