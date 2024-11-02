#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

#include "grayscale.h"
#include "gaussian_blur.h"

int main(int argc, char** argv)
{
    //double **ker = get_kernel_mat(3,0.84089642);
    double val = get_gaussian_value(0,1, 0.84089642);
    printf("%f\n", val);
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
    surface_to_blur(surface, 3, 3);

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
