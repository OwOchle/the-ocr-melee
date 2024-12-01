#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

#include "gaussian_binary.h"
#include "gaussian_blur.h"
#include "grayscale.h"
#include "line_detection.h"
#include "objects_detection.h"
#include "sobel.h"
#include "threshold.h"

void save_surface(const char* file_name, SDL_Surface* image) {
    IMG_SavePNG(image, file_name);
}

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("Line Detection", 0, 0, 0, 0,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

    int height = surface->h, width = surface->w;

    SDL_SetWindowSize(window,  surface->w, surface->h);

    surface_to_grayscale(surface);

    // surface_to_threshold(surface, 240); // Adjust the threshold as needed
    // surface_to_gaussian_binary(surface, 2, 2, 3);
    surface_to_simple_binary(surface, 128);

    save_surface("../outputs/output_threshold.png", surface);

    surface_to_objects(surface);

    save_surface("../outputs/output_dfs.png", surface);

    printf("image_processing: Saved threshold file in outputs folder.\n");

    surface_to_blur(surface, 3, 2);

    save_surface("../outputs/output_blur.png", surface);

    printf("image_processing: Saved blured file in outputs folder.\n");


    float* gradient_magnitude = malloc(width * height * sizeof(float));
    float* gradient_direction = malloc(width * height * sizeof(float));
    
    surface_to_sobel(surface, gradient_magnitude, gradient_direction);

    save_surface("../outputs/output_sobel.png", surface);

    printf("image_processing: Saved sobel file in outputs folder.\n");
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    detect_grid_lines_y(surface, surface, 5);
    detect_grid_lines_x(surface, surface, 4);

    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    free(gradient_magnitude);
    free(gradient_direction);


    save_surface("../outputs/output_lines.png", surface);

    printf("image_processing: Saved lines file in outputs folder.");

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
