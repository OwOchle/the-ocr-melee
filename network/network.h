#pragma once
#include <stdint.h>

typedef struct {
    float weight;
    
    Node *exiting;
    Node *entering;
} Edge;

typedef struct {
    float bias;

    Edge **entering; // List of edges, size of before layer
    Edge **exiting; // List of edges, size of after layer
} Node;

typedef struct {
    Node **nodes; // List of nodes
    int16_t nodeCount;
} Layer;

typedef struct {
    Layer **layers; // List of layers
    char layerCount;
} Network;