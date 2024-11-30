#include "objects_detection.h"
#include "utils/linked_list.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

void surface_dfs(SDL_Surface *surface) {}

linkedList *surface_to_objects(SDL_Surface *surface)
{
    int height = surface->h;
    int width = surface->w;

    // Create a temporary surface for the result
    SDL_Surface *marks_surface = SDL_CreateRGBSurface(
        0, width, height, 32, surface->format->Rmask, surface->format->Gmask,
        surface->format->Bmask, surface->format->Amask
    );

    // Process each pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint32 *pixel =
                (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch +
                           x * surface->format->BytesPerPixel);

            Uint8 r, g, b;
            SDL_GetRGB(*pixel, surface->format, &r, &g, &b);

            printf("x: %i y: %i | R: %i, G: %i, B: %i\n", x, y, r, g, b);

            // Uint32 *target_pixel = (Uint32 *) ((Uint8 *) temp_surface->pixels
            // +
            //                                   y * temp_surface->pitch +
            //                                   x *
            //                                   temp_surface->format->BytesPerPixel);
            //*target_pixel = new_pixel;
        }
    }

    // Copy back to original surface
    SDL_BlitSurface(marks_surface, NULL, surface, NULL);
    SDL_FreeSurface(marks_surface);
}