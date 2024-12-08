#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "word_search.h"
#include "utils/array.h"

#define array_get_as_matrix(array, width, x, y) array[y * width + x]

size_t count_length_until(char *input, char sep)
{
    size_t s = 0;
    while (*input != '\0' && *input != sep)
    {
        input++;
        s++;
    }

    return s;
}

mixed_words_t *mixed_words_from_file(char *path)
{
    FILE *f = fopen(path, "r");

    if (f == NULL)
    {
        errx(1, "Could not load '%s'", path);
    }

    struct stat stats;

    fstat(f->_fileno, &stats);

    char *file = malloc(stats.st_size + 1);

    file[stats.st_size] = 0;

    size_t jr;
    size_t i = 0;
    while ((jr = fread(file + i, sizeof(char), 2048, f)) != 0)
    {
        i += jr;
    }

    puts(file);

    fclose(f);

    size_t file_pos = 0;

    mixed_words_t *mw = malloc(sizeof(mixed_words_t));

    size_t width_length = count_length_until(file, '\n');
    char width_string[width_length + 1] = {};
    strncpy(width_string, file, width_length);
    file_pos += width_length + 1;

    mw->width = atoll(width_string);

    printf("width: %d\n", mw->width);

    size_t height_length = count_length_until(file + file_pos, '\n');
    char height_string[height_length + 1] = {};
    strncpy(height_string, file + file_pos, height_length);
    file_pos += height_length + 1;

    mw->height = atoll(height_string);

    printf("height: %d\n", mw->height);

    mw->grid = malloc(sizeof(char) * mw->width * mw->height);

    for (size_t line = 0; line < mw->height; line++)
    {
        for (size_t col = 0; col < mw->width; col++)
        {
            array_get_as_matrix(mw->grid, mw->width, col, line) = file[file_pos];
            file_pos++;
        }
        file_pos++;
    }

    array_print(mw->width * mw->height, mw->grid);

    size_t word_count_length = count_length_until(file + file_pos, '\n');
    char word_count_string[word_count_length + 1] = {};
    strncpy(word_count_string, file + file_pos, word_count_length);
    file_pos += word_count_length + 1;

    mw->word_count = atoll(word_count_string);

    printf("word_count: %d\n", mw->word_count);

    mw->word_list = malloc(sizeof(char *) * mw->word_count);

    for (size_t w = 0; w < mw->word_count; w++)
    {
        size_t word_length = count_length_until(file + file_pos, '\n');
        char *word = malloc(sizeof(char) * word_length + 1);
        word[word_length] = 0;
        strncpy(word, file + file_pos, word_length);

        mw->word_list[w] = word;

        file_pos += word_length + 1;
    }

    free(file);

    return mw;
}

void mixed_words_free(mixed_words_t *mw)
{
    free(mw->grid);
    for (size_t i = 0; i < mw->word_count; i++)
    {
        free(mw->word_list[i]);
    }

    free(mw->word_list);
    free(mw);
}