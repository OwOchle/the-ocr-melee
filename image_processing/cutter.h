#pragma once

#include <SDL2/SDL.h>

SDL_Surface *crop_surface(SDL_Surface *input, size_t x, size_t y, size_t width, size_t height);