#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat *format){
    Uint8 r = pixel_color & 0xFF;
    Uint8 g = pixel_color & 0xFF;
    Uint8 b = pixel_color & 0xFF;

    // SDL_GetRGB(pixel_color, format, &r, &g, &b);
    uint factor = 1.9;
    return SDL_MapRGB(format, r*factor, g*factor, b*factor);
    // return SDL_MapRGB(format, r*61/256, g*174/256, b*21/256);
}

void surface_to_grayscale(SDL_Surface *surface){
    Uint32 pixel = SDL_MapRGB(surface->format, 255, 255, 255);

    for (size_t y = 0; y < surface->h; y++)
    {
        for (size_t x = 0; x < surface->w; x++)
        {
            Uint32 * target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
                                             + y * surface->pitch
                                             + x * surface->format->BytesPerPixel);
            * target_pixel = pixel_to_grayscale(*target_pixel, surface->format);
        } 
    }
}