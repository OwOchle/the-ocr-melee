#pragma once

#include <stdlib.h>

#include "word_search.h"

struct output {
    size_t width;
    size_t height;
    size_t word_count;
    size_t word_lengths[32];
};

struct output get_output_from_proc(char *tmp_dir, char *image);

mixed_words_t *ocr_letters(char *tmp_dir, char *network, struct output out);

void solver_action(char *grid, mixed_words_t *to_solve);