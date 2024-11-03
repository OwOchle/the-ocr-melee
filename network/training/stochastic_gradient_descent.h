#include "../network.h"

#include "../network_utils/batch.h"
#include "../network_utils/gradiant.h"

#include <stdint.h>

int stochastic_gradiant_descent(
    Network *network, Batch *batch, size_t epochs, uint16_t mini_batch_size,
    float eta, float lambda, Batch *validation_batch
);

Batch **create_mini_batches(
    Network *network, Batch *batch, uint16_t mini_batch_size,
    uint16_t *num_mini_batches
);
