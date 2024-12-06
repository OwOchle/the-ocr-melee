#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "file_io.h"
#include "utils/array.h"
#include "utils/matrix.h"
#include "evaluate.h"

#define get_pixel(surface, x, y) ((Uint8 *)surface->pixels + (y) * surface->pitch + (x) * surface->format->BytesPerPixel)

#define IMAGE_SIZE 20



float *image_to_matrix(char *path)
{
    SDL_Surface *image = IMG_Load(path);

    if (image == NULL)
    {
        errx(1, "Could not load image at path '%s'", path);
    }

    float *out = malloc(sizeof(float) * IMAGE_SIZE * IMAGE_SIZE);
    for (size_t y = 0; y < image->h; y++)
    {
        for (size_t x = 0; x < image->w; x++)
        {
            Uint8 *pix = get_pixel(image, x, y);
            array_get_as_matrix(out, IMAGE_SIZE, x, y) = (pix[0] + pix[1] + pix[2]) ? 1.0 : 0.0;
        }
    }

    SDL_FreeSurface(image);

    return out;
}


int main(int argc, char **argv)
{
    if (argc < 3)
    {
        errx(1, "Not enough arguments\nUsage: ./network <network path> <letter image path>");
    }

    Network *network;
    
    if (network_read(&network, argv[1]))
    {
        errx(1, "Could not read network at path '%s'", argv[1]);
    }

    float *image = image_to_matrix(argv[2]);

    verbose_print_matrix_float(image, IMAGE_SIZE, IMAGE_SIZE);

    float *res = feedforward(network, image);

    size_t size = network_last_layer_count(network);

    size_t index = array_max_index_float(size, res);

    printf("My guess is telling me the letter is an %c\n", 'A' + index);

    free(res);
    network_free(network);
    free(image);

    return 0;
}
