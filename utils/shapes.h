#pragma once

#include "linked_list.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define sbb_width(bb) bb->max_x - bb->min_x
#define sbb_height(bb) bb->max_y - bb->min_y
#define sbb_area(bb) (sbb_width(bb)) * (sbb_height(bb))

#define split_bb(surface, bb) crop_surface(surface, bb->min_x, bb->min_y, sbb_width(bb), sbb_height(bb))

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
linkedList *
find_shapes_in_boundings(linkedList *shapes, ShapeBoundingBox *target_box);
linkedList* detect_unique_shapes(linkedList* shapes);
SDL_Surface **shapes_to_surfaces(linkedList *shapes, size_t *length) ;

linkedList *find_shapes_in_y_delta(linkedList *shapes, ShapeBoundingBox *control, size_t delta);
linkedList *find_shapes_in_x_delta(linkedList *shapes, ShapeBoundingBox *control, size_t delta);
Node *find_top_left(linkedList *shapes);
ShapeBoundingBox *find_nearest_right(linkedList *shapes, ShapeBoundingBox *control);
ShapeBoundingBox *find_nearest_down(linkedList *shapes, ShapeBoundingBox *control);