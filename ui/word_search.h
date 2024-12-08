#pragma once

#include <stdlib.h>

typedef struct MixedWords {
    size_t width;
    size_t height;
    char *grid;

    size_t word_count;
    char **word_list;
} mixed_words_t;

mixed_words_t *mixed_words_from_file(char *path);

void mixed_words_free(mixed_words_t *mw);