#include "utils/matrix.h"
#include "read_image.h"
#include "utils/verbose.h"

#include <err.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        errx(1, "Usage: ./training <image_path>");
    }

    size_t count;

    InputImage *output = load_directory(argv[1], &count);
    
    if (output == NULL)
    {
        errx(1, "output is null");
    }


    int index = 52;
    verbose_printf("name: %s, character: %c, category: %d\n", output[index].name, output[index].character, output[index].category);
    verbose_print_matrix_uchar(output[index].image, IMAGE_SIZE, IMAGE_SIZE);

    free(output);
    
    return 0;
}
