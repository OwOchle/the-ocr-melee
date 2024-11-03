#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>

#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

void get_sobel_kernels(int (*kernel_x)[3][3], int (*kernel_y)[3][3]);

void surface_to_sobel(SDL_Surface *surface, float* gradient_magnitude, float* gradient_direction);