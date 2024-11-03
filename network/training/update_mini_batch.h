#pragma once

#include "../network.h"
#include "../network_utils/batch.h"

int update_mini_batch(
    Network *network, const Batch *mini_batch, float eta, float lambda,
    int total_training_size
);
