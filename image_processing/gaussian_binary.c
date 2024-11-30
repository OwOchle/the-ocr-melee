#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>


void surface_to_gaussian_binary(SDL_Surface *surface, int maxValue, int radius, int C){
 int height = surface->h;
    int width = surface->w;

    // Create a temporary surface for the result
    SDL_Surface *temp_surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                     surface->format->Rmask,
                                                     surface->format->Gmask,
                                                     surface->format->Bmask,
                                                     surface->format->Amask);

    // Allocate memory for Gaussian kernel
    double *kernel = malloc((2 * radius + 1) * (2 * radius + 1) * sizeof(double));
    
    // Create Gaussian kernel
    double sigma = radius / 3.0;
    double kernel_sum = 0.0;
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            double exponent = -(i*i + j*j) / (2 * sigma * sigma);
            kernel[(i + radius) * (2 * radius + 1) + (j + radius)] = exp(exponent);
            kernel_sum += kernel[(i + radius) * (2 * radius + 1) + (j + radius)];
        }
    }

    // Normalize kernel
    for (int i = 0; i < (2 * radius + 1) * (2 * radius + 1); i++) {
        kernel[i] /= kernel_sum;
    }

    // Process each pixel
    for (int y = radius; y < height - radius; y++) {
        for (int x = radius; x < width - radius; x++) {
            double weighted_mean_r = 0.0, weighted_mean_g = 0.0, weighted_mean_b = 0.0;
            double local_pixel_r = 0.0, local_pixel_g = 0.0, local_pixel_b = 0.0;

            // Apply Gaussian kernel to calculate weighted mean
            for (int i = -radius; i <= radius; i++) {
                for (int j = -radius; j <= radius; j++) {
                    Uint32 *pixel_ptr = (Uint32 *) ((Uint8 *) surface->pixels +
                                                    (y + j) * surface->pitch +
                                                    (x + i) * surface->format->BytesPerPixel);

                    Uint8 r, g, b;
                    SDL_GetRGB(*pixel_ptr, surface->format, &r, &g, &b);

                    double kernel_weight = kernel[(i + radius) * (2 * radius + 1) + (j + radius)];
                    
                    // Weighted mean calculation
                    weighted_mean_r += r * kernel_weight;
                    weighted_mean_g += g * kernel_weight;
                    weighted_mean_b += b * kernel_weight;

                    // Get the current pixel values
                    if (i == 0 && j == 0) {
                        local_pixel_r = r;
                        local_pixel_g = g;
                        local_pixel_b = b;
                    }
                }
            }

            // Adaptive thresholding
            Uint8 thr_r = (local_pixel_r < (weighted_mean_r - C)) ? 255 : 0;
            Uint8 thr_g = (local_pixel_g < (weighted_mean_g - C)) ? 255 : 0;
            Uint8 thr_b = (local_pixel_b < (weighted_mean_b - C)) ? 255 : 0;

            Uint32 new_pixel = SDL_MapRGB(surface->format, thr_r, thr_g, thr_b);
            Uint32 *target_pixel = (Uint32 *) ((Uint8 *) temp_surface->pixels +
                                               y * temp_surface->pitch +
                                               x * temp_surface->format->BytesPerPixel);
            *target_pixel = new_pixel;
        }
    }

    // Free kernel memory
    free(kernel);

    // Copy back to original surface
    SDL_BlitSurface(temp_surface, NULL, surface, NULL);
    SDL_FreeSurface(temp_surface);
}