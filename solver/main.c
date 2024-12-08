#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "solver.h"
#include "file_reader.h"
#include "helpers.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr,
        "Usage: ./image_processing <grid file> <word>\n"
        "       Arguments:\n"
        "           input image: Input image path\n"
        "           step: 'grayscale', 'binary', 'shapes', 'sobel'\n"
        "           output: Output image path or '-' for stdout\n"
        );
        errx(1, "Invalid number of argument. expected 4, got : %d", argc);
        return 1;
    }

    struct Grid *grid = load_grid(argv[1]);

    print_grid(grid);

    char *word = malloc(sizeof(char) * strlen(argv[2]) + 1);

    strcpy(word, argv[2]);

    for (char *p = word ; *p; ++p) *p = tolower(*p);

    word_coord res;
    if (!find_word_matrix(grid->grid, grid->height, grid->width, word, strlen(word), &res))
    {
        printf("Not found\n");
    }
    else
    {
        printf("(%d,%d)(%d,%d)\n", res.ystart, res.xstart, res.yend, res.xend);
    }

    free(word);
    free(grid->grid);
    free(grid);

    return 0;
}