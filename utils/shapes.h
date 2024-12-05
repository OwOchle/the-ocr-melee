#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "linked_list.h"

typedef struct ShapeBoundingBox
{
    int max_x;
    int max_y;
    int min_x;
    int min_y;
    int center_x;
    int center_y;
} ShapeBoundingBox;


//void show_bounding_box(SDL_Surface *surface, int x1, int y1, int x2, int y2, SDL_Color color);
ShapeBoundingBox* get_shape_boundings(linkedList *shape);

void show_shape_boundings(SDL_Surface *surface, linkedList* shape, SDL_Color color);
void show_shapes_boundings(SDL_Surface *surface, linkedList* shape, SDL_Color color);

bool is_in_shape_bounds(int x, int y, ShapeBoundingBox* shape_boudings);
bool is_in_shape(int x, int y, linkedList* shape);

