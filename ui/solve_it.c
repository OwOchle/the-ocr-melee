#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <ctype.h>
#include <wait.h>

#include "solve_it.h"
#include "word_search.h"

#ifndef IMAGE_PROCESSING_EXE
#define IMAGE_PROCESSING_EXE "../image_processing/image_processing"
#endif

#ifndef NETWORK_EXE
#define NETWORK_EXE "../network/network"
#endif

#ifndef SOLVER_EXE
#define SOLVER_EXE "../solver/solver"
#endif

#define array_get_as_matrix(array, width, x, y) array[y * width + x]

struct output get_output_from_proc(char *tmp_dir, char *image)
{
    int image_pipe[2];

    if (pipe(image_pipe))
    {
        errx(1, "error opening pipe");
    }

    struct output out;

    if (!fork())
    {
        close(image_pipe[0]);
        dup2(image_pipe[1], STDOUT_FILENO);

        char *arguments[5] = {IMAGE_PROCESSING_EXE, image, "full", tmp_dir, NULL};

        execvp(IMAGE_PROCESSING_EXE, arguments);
        exit(1);
    }
    else
    {
        close(image_pipe[1]);

        read(image_pipe[0], &out, sizeof(struct output));

        close(image_pipe[0]);
    }

    printf("got w: %lu, h: %lu", out.width, out.height);
    fflush(stdout);
    return out;
}

char get_letter_from_ocr(char *network, char *image)
{
    int pip[2];

    pipe(pip);
    char result;

    if (!fork())
    {
        close(pip[0]);
        dup2(pip[1], STDOUT_FILENO);

        char *arguments[4] = {NETWORK_EXE, network, image, NULL};

        execvp(NETWORK_EXE, arguments);
        exit(1);
    }
    else
    {
        close(pip[1]);
        
        read(pip[0], &result, 1);

        close(pip[0]);
    }

    printf("found letter %c\n", result);

    return tolower(result);
}

mixed_words_t *ocr_letters(char *tmp_dir, char *network, struct output out)
{
    mixed_words_t *output = malloc(sizeof(mixed_words_t));
    output->width = out.width;
    output->height = out.height;
    output->grid = malloc(sizeof(char) * out.width * out.height);

    char *name;

    for (size_t y = 0; y < out.height; y++)
    {
        for (size_t x = 0; x < out.width; x++)
        {
            asprintf(&name, "%s/grid_%lu_%lu.png", tmp_dir, x, y);

            array_get_as_matrix(output->grid, output->width, x, y) = get_letter_from_ocr(network, name);

            free(name);
        }
    }

    for (size_t i = 0; i < output->width * output->height; i++)
    {
        printf("%c ", output->grid[i]);
    }

    output->word_count = out.word_count;

    output->word_list = malloc(sizeof(char *) * out.word_count);

    for (size_t i = 0; i < out.word_count; i++)
    {
        char *wor = malloc(out.word_lengths[i] + 1);

        wor[out.word_lengths[i]] = 0;

        for (size_t let = 0; let < out.word_lengths[i]; let++)
        {
            asprintf(&name, "%s/word_%lu_letter_%lu.png", tmp_dir, i, let);

            printf("%s\n", name);

            wor[let] = get_letter_from_ocr(network, name);

            free(name);
        }

        output->word_list[i] = wor;
    }

    printf("word 1: %s\n", output->word_list[0]);

    return output;
}

void solver_action(char *grid, mixed_words_t *to_solve)
{
    for (size_t i = 0; i < to_solve->word_count; i++)
    {
        if (!fork())
        {
            printf("%s: \n", to_solve->word_list[i]);
            char *arguments[4] = {SOLVER_EXE, grid, to_solve->word_list[i], NULL};

            execvp(SOLVER_EXE, arguments);
            exit(1);
        }
        else
        {
            int status;
            wait(&status);
        }
    }
}