#include "objects_detection.h"
#include "utils/linked_list.h"
#include "utils/shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "utils/verbose.h"

const int MARK_COLOR_R = 168;
const int MARK_COLOR_G = 157;
const int MARK_COLOR_B = 20;

SDL_Color UNMARKED_COLOR = {89, 67, 167}; // Purple (Not marked)

bool is_pixel_colored(SDL_Surface *marks_surface, int x, int y, SDL_Color color)
{
    Uint32 *mark_pixel =
        (Uint32 *)((Uint8 *)marks_surface->pixels + y * marks_surface->pitch +
                   x * marks_surface->format->BytesPerPixel);
    Uint8 r, g, b;
    SDL_GetRGB(*mark_pixel, marks_surface->format, &r, &g, &b);
    if (r == color.r && g == color.g && b == color.b)
    {
        return true;
    }
    return false;
}

int mark_box(
    SDL_Surface *surface, ShapeBoundingBox *bounding_box, linkedList *shape,
    SDL_Color bound_color
)
{
    show_shape_boundings(surface, shape, bound_color);
    // Mark the center;
    // printf("mark ze box\n");
    if (bounding_box->center_x != NULL && bounding_box->center_y != NULL){
        Uint32 *mark_pixel =
        (Uint32 *)((Uint8 *)surface->pixels + bounding_box->center_y * surface->pitch +
                   bounding_box->center_x * surface->format->BytesPerPixel);
        *mark_pixel = SDL_MapRGB(surface->format, MARK_COLOR_R, MARK_COLOR_G, MARK_COLOR_B);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

bool is_pixel_marked(SDL_Surface *marks_surface, int x, int y)
{
    if (x >= marks_surface->w || y >= marks_surface->h)
    {
        printf(
            "X: %i, Y: %i out of bounds (%i,%i)\n", x, y, marks_surface->w,
            marks_surface->h
        );
    }
    Uint32 *mark_pixel =
        (Uint32 *)((Uint8 *)marks_surface->pixels + y * marks_surface->pitch +
                   x * marks_surface->format->BytesPerPixel);
    Uint8 r, g, b;
    SDL_GetRGB(*mark_pixel, marks_surface->format, &r, &g, &b);
    if (r == MARK_COLOR_R && g == MARK_COLOR_G && b == MARK_COLOR_B)
    {
        return true;
    }
    return false;
}

bool is_marked(SDL_Surface* marks_surface, ShapeBoundingBox* bounding_box){
    if (bounding_box->center_x != NULL && bounding_box->center_y != NULL){
        return is_pixel_marked(
            marks_surface, bounding_box->center_x, bounding_box->center_y
        );
    }
    return EXIT_FAILURE;
}

void letter_dfs_by_similar(
    SDL_Surface *marks_surface, linkedList *shape, linkedList *shapes,
    linkedList *dest, int depth, SDL_Color group_color
)
{
    const int MAX_DEPTH = 3000;
    if (depth > MAX_DEPTH)
    {
        return;
    }
    if (shape == NULL)
    {
        printf("Shape Null quit\n");
        return;
    }
    int width = marks_surface->w;
    int height = marks_surface->h;

    ShapeBoundingBox *box = get_shape_boundings(shape);

    // If pixel is already marked (blue), return
    if (is_marked(marks_surface, box))
    {
        return;
    }

    // Mark
    int x = box->center_x;
    int y = box->center_y;
    mark_box(marks_surface, box, shape, group_color);

    int shape_width = box->max_x - box->min_x;
    int shape_height = box->max_y - box->min_y;

    int margin_y = 4 * shape_height;
    int margin_x = 6 * shape_width;

    for (int i = 0; i < marks_surface->w - x - 1 && i <= margin_x;
         i++) // Positive left
    {
        if (!is_pixel_marked(marks_surface, x + i, y))
        {
            linkedList *shape = find_shape_containing_point(x + i, y, shapes);

            if (shape != NULL)
            {
                ShapeBoundingBox *box = get_shape_boundings(shape);
                int shape_width2 = box->max_x - box->min_x;
                int shape_height2 = box->max_y - box->min_y;

                if (shape_height >= shape_height2 * 0.4 &&
                    shape_height <= shape_height2 * 1.5)
                {
                    letter_dfs_by_similar(
                        marks_surface, shape, shapes, dest, depth += 1,
                        group_color
                    );
                }
            }
        }
    }
    for (int i = 0; i < marks_surface->h - y - 1 && i <= margin_y;
         i++) // Positive left
    {
        if (!is_pixel_marked(marks_surface, x, y + i))
        {
            linkedList *shape = find_shape_containing_point(x, y + i, shapes);

            if (shape != NULL)
            {
                ShapeBoundingBox *box = get_shape_boundings(shape);
                int shape_width2 = box->max_x - box->min_x;
                int shape_height2 = box->max_y - box->min_y;

                if (shape_height >= shape_height2 * 0.4 &&
                    shape_height <= shape_height2 * 1.5)
                {
                    letter_dfs_by_similar(
                        marks_surface, shape, shapes, dest, depth += 1,
                        group_color
                    );
                }
            }
        }
    }

    free(box);
}

ShapeBoundingBox *get_shape_groups(SDL_Surface *surface, linkedList *shapes)
{
    SDL_Color color = {89, 67, 167}; // Purple (Not marked)
    show_shapes_center(surface, shapes, color);

    int height = surface->h;
    int width = surface->w;

    // Marks surface
    SDL_Surface *marks_surface = SDL_CreateRGBSurface(
        0, width, height, 32, surface->format->Rmask, surface->format->Gmask,
        surface->format->Bmask, surface->format->Amask
    );
    SDL_BlitSurface(
        surface, NULL, marks_surface, NULL
    ); // print the surface into the marks one.
    int i = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (1 == 1)
            {
                if (!is_pixel_marked(marks_surface, x, y) &&
                    is_pixel_colored(marks_surface, x, y, UNMARKED_COLOR))
                {
                    i++;
                    printf("X: %i, y : %i\n", x, y);
                    SDL_Color color2 = {
                        rand() % 255, rand() % 255, rand() % 255
                    };

                    linkedList *shape_list = list_create();
                    linkedList *shape =
                        find_shape_containing_point(x, y, shapes);
                    letter_dfs_by_similar(
                        marks_surface, shape, shapes, shape_list, 0, color2
                    );
                }
            }
        }
    }
    SDL_BlitSurface(marks_surface, NULL, surface, NULL);
}

void merge_histogram_lines(int *histogram, int histogram_size, int threshold) {
    int *merged_histogram = calloc(histogram_size, sizeof(int));
    
    for (int i = 0; i < histogram_size; i++) {
        // Skip already processed lines
        if (histogram[i] == 0) continue;
        
        // Start merging lines
        int current_line_points = histogram[i];
        int current_line_position = i;
        
        // Look for nearby lines to merge
        for (int j = i + 1; j < histogram_size; j++) {
            // If line is within threshold and has points
            if (histogram[j] > 0 && abs(j - i) <= threshold) {
                current_line_points += histogram[j];
                histogram[j] = 0; // Mark as processed
            }
        }
        
        // Store merged line
        merged_histogram[current_line_position] = current_line_points;
    }
    
    // Copy back to original histogram
    memcpy(histogram, merged_histogram, histogram_size * sizeof(int));
    
    free(merged_histogram);
}

void shapes_center_histogram(SDL_Surface* surface, linkedList* shapes) {
    const int w = surface->w;
    const int h = surface->h;

    int *histo_x = calloc(w, sizeof(int)); // Vertical lines
    int *histo_y = calloc(h, sizeof(int)); // Horizontal lines

    Node *elm = shapes->head;
    while (elm->next != NULL) {
        if (elm->x != -42 && elm->y != -42) {
            ShapeBoundingBox* box = get_shape_boundings(elm->shape);
            elm->shape_bounding_box = box;

            if (box != NULL) {
                int center_x = elm->shape_bounding_box->center_x;
                int center_y = elm->shape_bounding_box->center_y;
                if (center_x >= 0 && center_y >= 0) {
                    histo_x[center_x]++;
                    histo_y[center_y]++;
                }
            }
        }
        elm = elm->next;
    }
    
    // Merge lines with a threshold of 5 pixels
    merge_histogram_lines(histo_y, h, 2);
    
    // Print merged histogram
    for (int i = 0; i < h; i++) {
        if (histo_y[i] >= 8) {
            verbose_printf("%i %i\n", histo_y[i], i);
        }
    }

    free(histo_x);
    free(histo_y);
}

void box_lines(linkedList* shapes){

}