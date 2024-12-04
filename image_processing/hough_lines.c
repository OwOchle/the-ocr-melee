#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "hough_lines.h"

#define deg2rad(x) (x * 0.0174532925199)
#define rad2deg(x) (x * 57.2957795131)

#define BIN_WIDTH 1
#define NUM_BINS 180 / BIN_WIDTH

#define array_get_as_matrix(array, width, x, y) array[y * width + x]

#define array_get_as_matrix_ptr(array, width, x, y) (&array[(y * width) + x])

#define as_double(x) ((double) x)

size_t hough_lines(bool *grid, size_t width, size_t height, line_t **lines)
{
    size_t max_dist = (int) sqrtf((float)(height * height) + (width * width));

    int theta;
    double rho;

    int votes[2*max_dist][NUM_BINS] = {};
    int line_threshold = 1000;

    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            if (array_get_as_matrix(grid, width, x, y))
            {
                for (theta = 0; theta <= 180; theta += BIN_WIDTH)
                {
                    rho = round(as_double(x) * cos(as_double(theta) - 90.0) + as_double(y) * sin(as_double(theta) - 90.0)) + max_dist;

                    votes[(long) rho][theta]++;
                }
            }
        }
    }

    size_t line_count = 0;

    for (size_t i = 0; i < 2*max_dist; ++i)
    {
        for (size_t j = 0; j < NUM_BINS; ++j)
        {
            if (votes[i][j] < line_threshold) continue;
            line_count++;
        }
    }

    size_t cur = 0;
    line_t *olines = malloc(sizeof(line_t) * line_count);

    for (size_t i = 0; i < 2*max_dist; ++i)
    {
        for (size_t j = 0; j < NUM_BINS; ++j)
        {
            if (votes[i][j] < line_threshold) continue;
            theta = j - 90;

            printf("Found line %d with rho %d and theta %d at %d %d\n", cur, rho, theta,i,j);
            olines[cur].theta = as_double(j) - 90.0;
            olines[cur].rho = as_double(i) - as_double(max_dist);
            cur++;
        }
    }

    *lines = olines;
    return line_count;
}