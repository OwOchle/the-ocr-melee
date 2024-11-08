#include "utils/matrix.h"
#include "read_image.h"

#include <err.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        errx(1, "Usage: ./training <image_path>");
    }

    uchar *img = load_image(argv[1]);
    
    if (img == NULL)
    {
        errx(1, "img is null");
    }
    
    verbose_print_matrix_uchar(img, IMAGE_SIZE, IMAGE_SIZE);
    
    free(img);
    
    return 0;
}
