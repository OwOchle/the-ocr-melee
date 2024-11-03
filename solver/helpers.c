#include <stdio.h>
#include <stdint.h>

#include "helpers.h"
#include "utils/verbose.h"

void print_grid(struct Grid *grid)
{
    for (uint16_t y = 0; y < grid->height; y++)
    {
        for (uint16_t x = 0; x < grid->width; x++)
        {
            verbose_printf("%c", grid->grid[y * grid->width + x]);
        }
        verbose_printf("\n");
    }
}