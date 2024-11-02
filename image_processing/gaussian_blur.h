#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>

double get_gaussian_value(int x, int y, double sigma);

double** get_kernel_mat(int radius, double sigma);

void surface_to_blur(SDL_Surface *surface, int radius, double sigma);