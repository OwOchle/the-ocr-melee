#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>

#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))


double get_gaussian_value(int x, int y, double sigma){

    double sigma2 = sigma*sigma;

    double exponent = exp(-((x*x)+(y*y))/(2*sigma2));
    

    return (exponent / (double)(2 * M_PI * sigma2));
}

double** get_kernel_mat(int radius, double sigma){
    int size = (2*radius)+1;
    double** kernel = calloc(size, sizeof(double*)); // Init the first row of the kernel

    for (int i = 0; i < size; i++)
    {
        kernel[i] = (double*)calloc(size, sizeof(double));
    }
    
    double sum = 0.0;

    for (int x = -radius; x < radius+1; x++)
    {
        for (int y = -radius; y < radius+1; y++)
        {
            double val = get_gaussian_value(x, y, sigma);

            kernel[x+radius][y+radius] = val;
            sum += val;
        }
    }
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}


void surface_to_blur(SDL_Surface *surface, int radius, double sigma){
    int height = surface->h;
    int width = surface->w;


    SDL_Surface *temp_surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                     surface->format->Rmask,
                                                     surface->format->Gmask,
                                                     surface->format->Bmask,
                                                     surface->format->Amask);

    double** kernel_mat = get_kernel_mat(radius, sigma);

    for (int x = radius; x < width-radius; x++)
    {
        for (int y = radius; y < width-radius; y++)
        {
            double weighted_sum_r = 0.0, weighted_sum_g = 0.0, weighted_sum_b = 0.0;

            // Apply the kernel to each channel
            for (int i = -radius; i <= radius; i++) {
                for (int j = -radius; j <= radius; j++) {
                    Uint32 *pixel_ptr = (Uint32 *) ((Uint8 *) surface->pixels +
                                                    (y + j) * surface->pitch +
                                                    (x + i) * surface->format->BytesPerPixel);

                    Uint32 pixel = *pixel_ptr;
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);

                    double kernel_value = kernel_mat[i + radius][j + radius];
                    weighted_sum_r += r * kernel_value;
                    weighted_sum_g += g * kernel_value;
                    weighted_sum_b += b * kernel_value;
                }
            }

            Uint32 new_pixel = SDL_MapRGB(surface->format,
                                          MIN(MAX((int)weighted_sum_r, 0), 255),
                                          MIN(MAX((int)weighted_sum_g, 0), 255),
                                          MIN(MAX((int)weighted_sum_b, 0), 255));
            Uint32 *target_pixel = (Uint32 *) ((Uint8 *) temp_surface->pixels +
                                               y * temp_surface->pitch +
                                               x * temp_surface->format->BytesPerPixel);
            *target_pixel = new_pixel;
        }
    }

    SDL_BlitSurface(temp_surface, NULL, surface, NULL);
    SDL_FreeSurface(temp_surface);

    for (int i = 0; i < (2 * radius + 1); i++) {
        free(kernel_mat[i]);
    }
    free(kernel_mat);
}
