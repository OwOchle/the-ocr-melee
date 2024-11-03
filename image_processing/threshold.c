#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>

void surface_to_threshold(SDL_Surface* surface, Uint16 threshold) {
    int width = surface->w;
    int height = surface->h;
    Uint32* pixels = (Uint32*)surface->pixels;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[y * width + x], surface->format, &r, &g, &b);
            int gray = (r + g + b) / 3;

            if (gray > threshold) {
                pixels[y * width + x] = SDL_MapRGB(surface->format, 255, 255, 255);
            } else {
                pixels[y * width + x] = SDL_MapRGB(surface->format, 0, 0, 0);
            }
        }
    }
}
