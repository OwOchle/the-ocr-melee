#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>

void get_sobel_kernels(int (*kernel_x)[3][3], int (*kernel_y)[3][3]);
void surface_to_sobel(SDL_Surface *surface);