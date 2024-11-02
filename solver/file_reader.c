#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "file_reader.h"
#include "utils/verbose.h"

#define BUFFER_SIZE 4

// Private function just to ensure file closure
struct Grid *__load_grid_file(FILE *file)
{
    size_t just_read;
    char buf[BUFFER_SIZE];
    size_t width = 0;
    size_t height = 0;

    while ((just_read = fread(buf, sizeof(char), BUFFER_SIZE, file)) > 0)
    {
        for (size_t i = 0; i < just_read; i++)
        {
            if (buf[i] == '\n')
            {
                height++;
                width = 0;
            }
            else
            {
                width++;
            }
        }
    }

    height++;

    verbose_printf("width: %d, height: %d\n", width, height);

    struct Grid *result = malloc(sizeof(struct Grid));
    result->width = width;
    result->height = height;
    result->grid = calloc(width * height, sizeof(char));

    rewind(file);

    size_t x = 0;
    size_t y = 0;

    while ((just_read = fread(buf, sizeof(char), BUFFER_SIZE, file)) > 0)
    {
        for (size_t i = 0; i < just_read; i++)
        {
            if (buf[i] == '\n')
            {
                y++;
                x = 0;
            }
            else
            {
                result->grid[y * width + x] = tolower(buf[i]);
                x++;
            }
        }
    }

    return result;
}

struct Grid *load_grid(char *path)
{
    if (path == NULL)
        return NULL;

    FILE *file = fopen(path, "r");

    if (file == NULL)
        return NULL;

    struct Grid *res = __load_grid_file(file);

    fclose(file);
    return res;
}