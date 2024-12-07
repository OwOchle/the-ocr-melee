#include "update_mini_batch.h"
#include "backpropagation.h"

#include "../../utils/array.h"
#include "../../utils/matrix.h"
#include "../network.h"
// #include "../../utils/threaded_matrix.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef float *Vector;
typedef float *Matrix;

int update_mini_batch(
    Network *network, const Batch *mini_batch, float eta, float lambda,
    int total_training_size
)
{
    if (mini_batch == NULL)
    {
        printf("Update_mini_batch Error: mini_batch is NULL\n");
        return 0;
    }

    // Constants
    const char layerCount = network->layerCount;
    const uint16_t input_size = network->entryCount;
    const uint16_t output_size = network->layers[layerCount - 1]->nodeCount;
    uint16_t batch_size = mini_batch->batchSize;

    // Update Time
    GradiantData *gradiant = gradiant_new(network);
    if (gradiant == NULL)
    {
        printf("An error occured: gradiant_new()\n");
        return 0;
    }

    for (size_t tupleIdx = 0; tupleIdx < batch_size; tupleIdx++)
    {
        Vector trainingData = mini_batch->layers[tupleIdx]->inputData;
        Vector desiredData = mini_batch->layers[tupleIdx]->outputData;

        GradiantData *deltaGradiant =
            backprop(network, trainingData, desiredData);

        

        // if (tupleIdx == 0)
        // {
        //     printf("\ndelta weigth:\n\n");
        //                 array_float_print(
        //         network->layers[0]->nodeCount * output_size,
        //         deltaGradiant->layers[0]->weights
        //     );
        //     printf("\n");
        //     array_float_print(
        //         network->layers[1]->nodeCount * output_size,
        //         deltaGradiant->layers[1]->weights
        //     );

        //     printf("\ndelta biais:\n\n");
        //     array_float_print(
        //         network->layers[0]->nodeCount, deltaGradiant->layers[0]->bias
        //     );
        //     printf("\n");
        //                 array_float_print(
        //         network->layers[1]->nodeCount, deltaGradiant->layers[1]->bias
        //     );
        // }

        // return 0;

        // nabla = nabla + gradiant
        for (size_t l = 0; l < layerCount; l++)
        {
            // printf("    - Adding layer %zu\n", l);
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

            matrix_add(
                pastNodeCount, nodeCount, gradiantLayer->weights, pastNodeCount,
                nodeCount, deltaGradiantLayer->weights
            );
        }

        gradiant_free(deltaGradiant);
    }

    for (size_t l = 0; l < layerCount; l++)
    {
        // printf("[[Training layer %zu]]\n", l);
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

        // printf(
        //     "Before weight[10][10]=%f\n",
        //     array_get_as_matrix(weightLayer, nodeCount, 10, 10)
        // );
        // printf(
        //     "weight calculation[0][0]= %f * %f - %f * %f\n",
        //     weightDecayFactor, array_get_as_matrix(weightLayer, nodeCount, 0,
        //     0), learningRate, array_get_as_matrix(gradiantWeightLayer,
        //     nodeCount, 0, 0)
        // );
        for (size_t y = 0; y < nodeCount; y++)
        {
            for (size_t x = 0; x < pastNodeCount; x++)
            {
                float *weight_ptr =
                    array_get_as_matrix_ptr(weightLayer, nodeCount, y, x);
                float gradiantWeight =
                    array_get_as_matrix(gradiantWeightLayer, nodeCount, y, x);
                // printf(
                //     "gradiantWeightLayer[%zu][%zu]= %f\n", y, x,
                //     gradiantWeight
                // );
                *weight_ptr = weightDecayFactor * (*weight_ptr) -
                              learningRate * gradiantWeight;
            }
        }
        // printf(
        //     "After weight[10][10]=%f\n",
        //     array_get_as_matrix(weightLayer, nodeCount, 10, 10)
        // );

        // printf(
        //     "\nBefore bias[10]=%f\n",
        //     array_get_as_matrix(weightLayer, nodeCount, 10, 0)
        // );

        // Bias Update
        Vector biasLayer = network->layers[l]->bias;
        Vector gradiantBiasLayer = gradiant->layers[l]->bias;

        for (size_t x = 0; x < nodeCount; x++)
        {
            float *bias = array_get_as_matrix_ptr(biasLayer, nodeCount, x, 0);
            float gradiantBias =
                array_get_as_matrix(gradiantBiasLayer, nodeCount, x, 0);

            *bias = (*bias) - learningRate * gradiantBias;
        }
        // printf(
        //     "After bias[10]=%f\n",
        //     array_get_as_matrix(weightLayer, nodeCount, 10, 0)
        // );
    }

    gradiant_free(gradiant);

    return 1;
}
