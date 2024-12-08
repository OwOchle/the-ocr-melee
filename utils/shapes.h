#pragma once

#include "linked_list.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct ShapeBoundingBox
{
    int max_x;
    int max_y;
    int min_x;
    int min_y;
    int center_x;
    int center_y;
} ShapeBoundingBox;

int round_to_upper_five_multiple(int num);

//void show_bounding_box(SDL_Surface *surface, int x1, int y1, int x2, int y2, SDL_Color color);
ShapeBoundingBox* get_shape_boundings(linkedList *shape);
void get_shape_center(int* x, int* y, ShapeBoundingBox* shape_boundings);

void show_shape_boundings(SDL_Surface *surface, linkedList* shape, SDL_Color color);
void show_shapes_center(
    SDL_Surface *surface, linkedList *shape, SDL_Color color
);
void show_boundings(
    SDL_Surface *surface, ShapeBoundingBox *box, SDL_Color color
);
void show_bounding_box(
    SDL_Surface *surface, int x1, int y1, int x2, int y2, SDL_Color color
);
void show_shapes_boundings(
    SDL_Surface *surface, linkedList *shapes, SDL_Color color
);

void show_shapes_center(
    SDL_Surface *surface, linkedList *shapes, SDL_Color color
);

bool is_in_shape_bounds(int x, int y, ShapeBoundingBox* shape_boudings);
bool is_in_shape(int x, int y, linkedList* shape);

linkedList* find_shape_containing_point(int x, int y, linkedList* shapes);
linkedList* detect_unique_shapes(linkedList* shapes);
SDL_Surface **shapes_to_surfaces(linkedList *shapes, size_t *length) ;
