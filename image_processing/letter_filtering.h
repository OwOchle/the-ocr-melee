#pragma once

#include "objects_detection.h"
#include "utils/linked_list.h"
#include "utils/shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdbool.h>


linkedList* filter_shapes(linkedList* shapes);

bool is_shape_in_main_grid(linkedList* shape, long avg_width, long avg_height, long avg_ratio, int width, int height, double ratio, linkedList* shapes);
