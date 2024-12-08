#include "objects_detection.h"
#include "utils/linked_list.h"
#include "utils/shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdbool.h>

int mark_box(SDL_Surface* surface, ShapeBoundingBox* bounding_box);

void letter_dfs_by_similar(
    SDL_Surface *marks_surface, linkedList *shape, linkedList *shapes,
    linkedList *dest, int depth
);
ShapeBoundingBox **get_shape_groups(SDL_Surface *surface, linkedList *shapes, int *size);
ShapeBoundingBox **get_shape_word_groups(
    SDL_Surface *surface, linkedList *shapes, int *size,
    ShapeBoundingBox *bounds
);