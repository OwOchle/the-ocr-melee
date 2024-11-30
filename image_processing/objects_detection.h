#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/linked_list.h"

linkedList *surface_to_objects(SDL_Surface *surface);