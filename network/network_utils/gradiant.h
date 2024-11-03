#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "../network.h"

typedef struct GradiantLayer
{
    float *weights;
    uint16_t nodeCount;
    float *bias;
} GradiantLayer;

typedef struct GradiantData
{
    uint16_t entryCount;
    char layerCount;
    GradiantLayer **layers;
} GradiantData;

GradiantData *gradiant_new(const Network *network);

void gradiant_free(GradiantData *gradiant);
