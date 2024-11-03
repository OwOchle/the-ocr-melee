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

int accuracy(Network *network, Batch *training_data);
int argmax(const float *array, uint16_t length);

float binary_accuracy(Network *network, Batch *batch);
