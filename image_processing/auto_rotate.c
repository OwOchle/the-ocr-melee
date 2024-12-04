#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "utils/linked_list.h"
#include "utils/shapes.h"
#include "auto_rotate.h"
#include "cutter.h"
#include "utils/array.h"
#include "hough_lines.h"
#include "sobel.h"

#define get_as_matrix_ptr(array, width, x, y) (array + (y * width) + x)
#define get_as_matrix(array, width, x, y) array[((y) * width) + (x)]

extern void save_surface(const char* file_name, SDL_Surface* image);

bool *shape_to_matrix(linkedList *shape)
{
    ShapeBoundingBox *bb = get_shape_boundings(shape);

    size_t width = bb->max_x - bb->min_x + 1;
    size_t height = bb->max_y - bb->min_y + 1;

    bool *result = calloc(width * height, sizeof(bool));

    Node *p = shape->head;

    while (p != NULL)
    {
        if (p->x == -42 || p->y == -42)
        {
            p = p->next;
            continue;
        }

        fflush(stdout);
        get_as_matrix(result, width, p->x - bb->min_x, p->y - bb->min_y) = true;
        p = p->next;
    }

    free(bb);

    return result;
}

bool is_a_matching_towards_right(bool *shape, ShapeBoundingBox *bb)
{
    // |>

    // Searching for tip
    size_t width = bb->max_x - bb->min_x + 1;
    size_t height = bb->max_y - bb->min_y + 1;

    bool last_pixel = get_as_matrix(shape, width, width-1, 0);

    int met_black = last_pixel == true;
    int met_white = last_pixel == false;

    for (size_t i = 0; i < height; i++)
    {
        if (get_as_matrix(shape, width, width-1, i))
        {
            if (!last_pixel)
            {
                last_pixel = true;
                met_black++;
            }
        }
        else
        {
            if (last_pixel)
            {
                last_pixel = false;
                met_white++;
            }
        }
    }

    printf("Met black: %d, met white: %d\n", met_black, met_white);

    if (met_black != 1 || met_white != 2)
    {
        printf("False\n");
        return false;
    }

    return true;
}

int is_a_matching(linkedList *shape)
{
    /*
        Element orientation
        0: not a
        1: left
        2: right
        3: up
        4: down
    */
    ShapeBoundingBox *bb = get_shape_boundings(shape);

    bool *grid = shape_to_matrix(shape);

    return is_a_matching_towards_right(grid, bb);

    free(bb);
    free(grid);

    return false;
}

bool *to_list(SDL_Surface *image)
{
    bool *result = malloc(image->w * image->h * sizeof(bool));

    for (size_t y = 0; y < image->h; y++)
    {
        for (size_t x = 0; x < image->w; x++)
        {
            Uint32 *input_pixel = image->pixels + 
                (y) * image->pitch + 
                (x) * sizeof(*input_pixel);

            get_as_matrix(result, image->w, x, y) = *input_pixel > 0 ? true : false;
        }
    }

    return result;
}

int determine_rotation(SDL_Surface *threasholded, linkedList *shapes)
{
    SDL_Surface *sobel = SDL_CreateRGBSurface(0, threasholded->w, threasholded->h, 32,
                                                threasholded->format->Rmask,
                                                threasholded->format->Gmask,
                                                threasholded->format->Bmask,
                                                threasholded->format->Amask);
    

    SDL_BlitSurface(threasholded, NULL, sobel, NULL);
    bool *img = to_list(sobel);

    SDL_FreeSurface(sobel);

    line_t *lines;

    size_t line_count = hough_lines(img, threasholded->w, threasholded->h, &lines);

    for (int i = 0; i < line_count; i++)
    {
        printf("%03d | angle: %f, distance: %f\n", i, lines[i].theta, lines[i].rho);
    }

    free(img);
    free(lines);
    return 0;
    ShapeBoundingBox *bouding_box;

    Node *p = shapes->head;

    while (p != NULL)
    {
        if (is_a_matching(p->shape))
        {
            ShapeBoundingBox *bb = get_shape_boundings(p->shape);
            char *name;
            asprintf(&name, "../outputs/matching_a/%d-%d-%d-%d.png", bb->min_x, bb->min_y, bb->max_x, bb->max_y);
            save_surface(name, crop_surface(threasholded, bb->min_x, bb->min_y, bb->max_x - bb->min_x, bb->max_y - bb->min_y));

            free(name);
            free(bb);
        }
        p = p->next;
    }

    return 0;
}