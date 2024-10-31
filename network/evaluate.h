#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "network.h"

/*
 * return the output of the network if `input_data` is an array with the same
 * size as the entryLayer
 */
float *feedforward(Network *network, float *input_data);
