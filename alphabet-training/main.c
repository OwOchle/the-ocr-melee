#include "utils/matrix.h"
#include "read_image.h"

#include <err.h>
#include <stdlib.h>

int main() 
{
    uchar *img = load_image("/disks/hdd2/epita/the-ocr-melee/misc/training-generator/outputs/sourgummy_y.bmp");
    
    if (img == NULL)
    {
        errx(1, "img is null");
    }
    
    verbose_print_matrix_uchar(img, IMAGE_SIZE, IMAGE_SIZE);
    
    free(img);
    
    return 0;
}
