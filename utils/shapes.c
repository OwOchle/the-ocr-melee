#include "shapes.h"
#include "linked_list.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void show_bounding_box(
    SDL_Surface *surface, int x1, int y1, int x2, int y2, SDL_Color color
)
{
    int left = (x1 < x2) ? x1 : x2;
    int right = (x1 < x2) ? x2 : x1;
    int top = (y1 < y2) ? y1 : y2;
    int bottom = (y1 < y2) ? y2 : y1;

    // Draw horizontal lines
    for (int x = left; x <= right; x++)
    {
        // Top line
        Uint32 pixelColor =
            SDL_MapRGB(surface->format, color.r, color.g, color.b);
        *((Uint32 *)surface->pixels + top * surface->pitch / 4 + x) =
            pixelColor;

        // Bottom line
        *((Uint32 *)surface->pixels + bottom * surface->pitch / 4 + x) =
            pixelColor;
    }

    // Draw vertical lines
    for (int y = top; y <= bottom; y++)
    {
        // Left line
        Uint32 pixelColor =
            SDL_MapRGB(surface->format, color.r, color.g, color.b);
        *((Uint32 *)surface->pixels + y * surface->pitch / 4 + left) =
            pixelColor;

        // Right line
        *((Uint32 *)surface->pixels + y * surface->pitch / 4 + right) =
            pixelColor;
    }
}

void show_shape_boundings(SDL_Surface *surface, linkedList* shape, SDL_Color color){
    ShapeBoundingBox* box = get_shape_boundings(shape);

    show_bounding_box(surface, box->max_x, box->max_y, box->min_x, box->min_y, color);

    free(box);
}

void show_shapes_boundings(SDL_Surface *surface, linkedList* shape, SDL_Color color){
    Node *elm = shape->head;
    if (shape-> head != NULL){
        while (elm->next != NULL)
    {
        if (elm->x != -42 && elm->y != -42)
        {
            show_shape_boundings(surface, elm->shape, color);
        }
        elm = elm->next;
    }
    }
}

ShapeBoundingBox* get_shape_boundings(linkedList *shape){
    int max_x = -1;
    int max_y = -1;
    int min_x = -1;
    int min_y = -1;
    Node *elm = shape->head;
    while (elm->next != NULL)
    {
        if (elm->x != -42 && elm->y != -42)
        {
            if (max_x == -1 && max_y == -1 && min_x == -1 &&
                min_y == -1)
            {
                max_x = elm->x;
                max_y = elm->y;
                min_x = elm->x;
                min_y = elm->y;
                continue;
            }
            if (elm->x > max_x)
                max_x = elm->x;
            if (elm->x < min_x)
                min_x = elm->x;
            if (elm->y < min_y)
                min_y = elm->y;
            if (elm->y > max_y)
                max_y = elm->y;
        }
        elm = elm->next;
    }
    ShapeBoundingBox* shape_box = malloc(sizeof(ShapeBoundingBox));
    shape_box->max_x = max_x;
    shape_box->max_y = max_y;
    shape_box->min_x = min_x;
    shape_box->min_y = min_y;
    return shape_box;
}
