#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "solver.h"
#include "file_reader.h"
#include "helpers.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        errx(1, "Invalid number of argument. expected 4, got : %d\n", argc);
        return 1;
    }

    struct Grid *grid = load_grid(argv[1]);

    print_grid(grid);

    word_coord *res = malloc(sizeof(word_coord));
    if (!find_word_matrix(grid->grid, grid->height, grid->width, argv[2], strlen(argv[2]), res))
    {
        free(res);
        free(grid->grid);
        free(grid);
        printf("Not found\n");
        return 0;
    }

    printf("(%d,%d)(%d,%d)\n", res->ystart, res->xstart, res->yend, res->xend);

    free(res);
    free(grid->grid);
    free(grid);

    return 0;
}