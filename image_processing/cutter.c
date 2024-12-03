#include <SDL2/SDL.h>
#include <stdio.h>

#include "cutter.h"

SDL_Surface *crop_surface(SDL_Surface *input, size_t in_x, size_t in_y, size_t width, size_t height)
{
    if (input == NULL)
    {
        return NULL;
    }

    if (input->w < (width + in_x) || input->h < (height + in_y))
    {
        return NULL;
    }

    SDL_Surface *out = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    if (out == NULL)
    {
        return NULL;
    }

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            Uint32 *input_pixel = input->pixels + 
                (y+in_y) * input->pitch + 
                (x+in_x) * sizeof(*input_pixel);

            Uint32 *output_pixel = out->pixels + (y * out->pitch) + (x * sizeof(*output_pixel));

            *output_pixel = *input_pixel;

            fflush(stdout);
        }
    }

    return out;
}