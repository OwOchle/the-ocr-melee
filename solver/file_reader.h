#pragma once

#include <stdint.h>

struct Grid {
    uint16_t width;
    uint16_t height;

    char *grid;
};

struct Grid *load_grid(char *path);