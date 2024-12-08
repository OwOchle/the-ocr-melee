#include <stdio.h>

#include "word_search.h"

int main(int argc, char **argv)
{
    mixed_words_t *mw = mixed_words_from_file(argv[1]);

    mixed_words_free(mw);

    return 0;
}