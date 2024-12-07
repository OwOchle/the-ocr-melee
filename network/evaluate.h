#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "network.h"
#include "network_utils/batch.h"

/*
 * return the output of the network if `input_data` is an array with the same
 * size as the entryLayer
 */
float *feedforward(Network *network, const float *input_data);

int argmax(const float *array, uint16_t length);
int accuracy(Network *network, Batch *training_data);

float total_cost(Network *network, Batch *batch, float lambda);