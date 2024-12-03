#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>

#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

void get_sobel_kernels(int (*kernel_x)[3][3], int (*kernel_y)[3][3]) {
    int temp_kernel_x[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int temp_kernel_y[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            (*kernel_x)[i][j] = temp_kernel_x[i][j];
            (*kernel_y)[i][j] = temp_kernel_y[i][j];
        }
    }
}

void surface_to_sobel(SDL_Surface *surface, float* gradient_magnitude, float* gradient_direction){
    int height = surface->h;
    int width = surface->w;

    int kernel_x[3][3];
    int kernel_y[3][3];
    get_sobel_kernels(&kernel_x, &kernel_y);

    int radius = 1;

    SDL_Surface *temp_surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                     surface->format->Rmask,
                                                     surface->format->Gmask,
                                                     surface->format->Bmask,
                                                     surface->format->Amask);

    for (int x = radius; x < width-radius; x++)
    {
        for (int y = radius; y < width-radius; y++)
        {
            double gx = 0.0, gy = 0.0;
            for (int i = -radius; i <= radius; i++)
            {
                for (int j = -radius; j <= radius; j++) {
                    if ((y+j) >= height || (x+i) >= width)
                    {
                        break;
                    }
                    
                    Uint32 pixel_val = *((Uint32 *)((Uint8 *)surface->pixels +
                                                    (y + j) * surface->pitch +
                                                    (x + i) * surface->format->BytesPerPixel));

                    Uint8 r, g, b;
                    SDL_GetRGB(pixel_val, surface->format, &r, &g, &b);
                    int gray = (r + g + b) / 3;
                    /*
                    gx_r += r * kernel_x[i + radius][j + radius];
                    gy_r += r * kernel_y[i + radius][j + radius];
                    gx_g += g * kernel_x[i + radius][j + radius];
                    gy_g += g * kernel_y[i + radius][j + radius];
                    gx_b += b * kernel_x[i + radius][j + radius];
                    gy_b += b * kernel_y[i + radius][j + radius];*/
                    gx += gray * kernel_x[i + radius][j + radius];
                    gy += gray * kernel_y[i + radius][j + radius];
                }
            }
            /*
            double gradient_r = sqrt((gx_r * gx_r) + (gy_r * gy_r));
            double gradient_g = sqrt((gx_g * gx_g) + (gy_g * gy_g));
            double gradient_b = sqrt((gx_b * gx_b) + (gy_b * gy_b));

            Uint8 final_r = (Uint8)MIN(MAX((int)gradient_r, 0), 255);
            Uint8 final_g = (Uint8)MIN(MAX((int)gradient_g, 0), 255);
            Uint8 final_b = (Uint8)MIN(MAX((int)gradient_b, 0), 255);

            Uint32 new_pixel = SDL_MapRGB(surface->format, final_r, final_g, final_b);*/

            float weighted_sum = sqrtf(gx * gx + gy * gy);
            float direction = atan2f(gy, gx);

            Uint8 final_pixel = (Uint8)MIN(MAX((int)weighted_sum, 0), 255);
            Uint32 new_pixel = SDL_MapRGB(surface->format, final_pixel, final_pixel, final_pixel);
            
            if (x < width && y < height){
                Uint32 *target_pixel = (Uint32 *)((Uint8 *)temp_surface->pixels +
                                              y * temp_surface->pitch +
                                              x * temp_surface->format->BytesPerPixel);
            *target_pixel = new_pixel;

            gradient_magnitude[y * width + x] = weighted_sum;
            gradient_direction[y * width + x] = direction;
            }
        }
    }
    
    SDL_BlitSurface(temp_surface, NULL, surface, NULL);
    SDL_FreeSurface(temp_surface);
}
