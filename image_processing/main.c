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

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("Surface_to_grayscale", 0, 0, 0, 0,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* t = IMG_Load(argv[1]);
    if (t == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(t, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(t);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_SetWindowSize(window,  surface->w, surface->h);

    surface_to_grayscale(surface);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_SUCCESS;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                        SDL_RenderCopy(renderer,texture,NULL,NULL);
                        SDL_RenderPresent(renderer);
                }
                break;
        }
    }
    return EXIT_SUCCESS;
}
