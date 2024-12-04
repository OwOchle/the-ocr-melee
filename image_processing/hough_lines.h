#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct line {
    float theta;
    float rho;
} line_t;

size_t hough_lines(bool *grid, size_t width, size_t height, line_t **lines);