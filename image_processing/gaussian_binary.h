#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

void surface_to_gaussian_binary(
    SDL_Surface *surface, int maxValue, int radius, int C
);

void surface_to_simple_binary(SDL_Surface *surface, int threshold);
