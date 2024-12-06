#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "shapes.h"
#include "linked_list.h"

#define get_pixel(surface, x, y) ((Uint32 *)surface->pixels + (y) * surface->pitch / 4 + (x))

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

void show_shape_boundings(
    SDL_Surface *surface, linkedList *shape, SDL_Color color
)
{
    ShapeBoundingBox *box = get_shape_boundings(shape);

    show_bounding_box(
        surface, box->max_x, box->max_y, box->min_x, box->min_y, color
    );

    free(box);
}

void show_shapes_boundings(
    SDL_Surface *surface, linkedList *shape, SDL_Color color
)
{
    Node *elm = shape->head;
    if (shape->head != NULL)
    {
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

ShapeBoundingBox *get_shape_boundings(linkedList *shape)
{
    int max_x = -1;
    int max_y = -1;
    int min_x = -1;
    int min_y = -1;
    Node *elm = shape->head;
    while (elm->next != NULL)
    {
        if (elm->x != -42 && elm->y != -42)
        {
            if (max_x == -1 && max_y == -1 && min_x == -1 && min_y == -1)
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
    ShapeBoundingBox *shape_box = malloc(sizeof(ShapeBoundingBox));
    shape_box->max_x = max_x;
    shape_box->max_y = max_y;
    shape_box->min_x = min_x;
    shape_box->min_y = min_y;
    return shape_box;
}

void get_shape_center(int *x, int *y, ShapeBoundingBox *shape_boundings)
{
    *x = (shape_boundings->max_x - shape_boundings->max_y) / 2;
    *y = (shape_boundings->max_y - shape_boundings->min_y) / 2;
}

bool is_in_shape_bounds(int x, int y, ShapeBoundingBox *shape_boudings)
{
    if ((x >= shape_boudings->min_x && x <= shape_boudings->max_x) &&
        (y >= shape_boudings->min_y && y <= shape_boudings->max_x))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_in_shape(int x, int y, linkedList *shape)
{
    ShapeBoundingBox *shape_boundings = get_shape_boundings(shape);

    bool status = is_in_shape_bounds(x, y, shape_boundings);

    free(shape_boundings);
    return status;
}

SDL_Surface **shapes_to_surfaces(linkedList *shapes, size_t *length) 
{
    SDL_Surface **out = malloc(sizeof(SDL_Surface *) * list_length(shapes));
    size_t c = 0;

    for (Node *p = shapes->head; p != NULL; p = p->next)
    {
        ShapeBoundingBox *bb = get_shape_boundings(p->shape);
        size_t w = bb->max_x - bb->min_x + 1;
        size_t h = bb->max_y - bb->min_y + 1;

        SDL_Surface *sur = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);

        SDL_Surface *out_surface = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);

        SDL_Rect rect = { 0, 0, w, h };

        SDL_FillRect(sur, &rect, 0xFFFFFFFF);

        for (Node *pix = p->shape->head; pix != NULL; pix = pix->next)
        {
            if (pix->x == -42 || pix->y == -42) continue;
            Uint32 *cur_pixel = get_pixel(sur, pix->x - bb->min_x, pix->y - bb->min_y);

            *cur_pixel = 0xFF000000;
        }

        SDL_Rect in_rect = {0,0,w,h};
        SDL_Rect out_rect = {0,0,20,20};

        SDL_BlitScaled(sur, &in_rect, out_surface, &out_rect);

        SDL_FreeSurface(sur);

        free(bb);

        out[c] = out_surface;

        c++;
    }

    *length = c;

    return out;
}