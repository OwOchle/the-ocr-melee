#include "objects_detection.h"
#include "utils/linked_list.h"
#include "utils/shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdbool.h>

int mark_box(SDL_Surface* surface, ShapeBoundingBox* bounding_box);

linkedList* letter_dfs_by_similar(Node* shape, linkedList* shapes, SDL_Surface* marks_surface);
void shapes_center_histogram(SDL_Surface* surface, linkedList* shapes);