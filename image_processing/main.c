#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <string.h>

#include "grayscale.h"
#include "gaussian_binary.h"
#include "objects_detection.h"
#include "utils/linked_list.h"
#include "utils/shapes.h"
#include "letter_filtering.h"
#include "grid_detection.h"
#include "sobel.h"

#define FP_SIZE 2097152
#define BUF_SIZE 2048

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr,
        "Usage: ./image_processing <input image> <step> <output>\n"
        "       Arguments:\n"
        "           input image: Input image path\n"
        "           step: 'grayscale', 'binary', 'shapes', 'sobel'\n"
        "           output: Output image path or '-' for stdout\n"
        "\n"
        "       Notes:\n"
        "           if step is 'full', output must be a path to a directory.\n"
        );
        exit(1);
    }

    SDL_Surface *input_image = IMG_Load(argv[1]);

    if (input_image == NULL)
    {
        errx(1, "Error while loading input image: %s", SDL_GetError());
    }

    if (!strcmp(argv[2], "grayscale"))
    {
        surface_to_grayscale(input_image);
    } 
    else if (!strcmp(argv[2], "binary"))
    {
        surface_to_grayscale(input_image);
        surface_to_simple_binary(input_image, 128);
    }
    else if (!strcmp(argv[2], "shapes"))
    {
        surface_to_grayscale(input_image);
        surface_to_simple_binary(input_image, 128);

        linkedList* shapes = surface_to_objects(input_image);
        linkedList* filtered_shapes = filter_shapes(shapes);

        SDL_Color color = {89, 67, 167};
        linkedList* isolated = detect_unique_shapes(filtered_shapes);

        show_shapes_boundings(input_image, isolated, color);

        shapes_center_histogram(input_image, filtered_shapes);
        
        list_free(isolated);
        list_free(filtered_shapes);
    }
    else if (!strcmp(argv[2], "sobel"))
    {
        float* gradient_magnitude = malloc(input_image->w * input_image->h * sizeof(float));
        float* gradient_direction = malloc(input_image->w * input_image->h * sizeof(float));

        surface_to_grayscale(input_image);
        surface_to_simple_binary(input_image, 128);
        surface_to_sobel(input_image, gradient_magnitude, gradient_direction);

        free(gradient_direction);
        free(gradient_magnitude);
    }
    else if (!strcmp(argv[2], "full"))
    {
        errx(1, "Not working yet")
    }
    else
    {
        errx(1, "Unknown step '%s'. Possible values are: grayscale, binary, shapes, sobel, full", argv[2]);
    }

    if (!strcmp(argv[3], "-"))
    {
        SDL_RWops *ops = SDL_RWFromFP(stdout, 1);

        if (IMG_SavePNG_RW(input_image, ops, 0))
        {
            errx(1, "Error while writing image: %s", SDL_GetError());
        }

        SDL_FreeRW(ops);
    }
    else
    {
        IMG_SavePNG(input_image, argv[3]);
    }

    SDL_FreeSurface(input_image);
}
