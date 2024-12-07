#include "objects_detection.h"
#include "utils/linked_list.h"
#include "utils/shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdbool.h>

#include "utils/verbose.h"

const int MARK_COLOR_R = 168;
const int MARK_COLOR_G = 157;
const int MARK_COLOR_B = 20;


int mark_box(SDL_Surface* surface, ShapeBoundingBox* bounding_box){
    // Mark the center;
    if (bounding_box->center_x != NULL && bounding_box->center_y != NULL){
        Uint32 *mark_pixel =
        (Uint32 *)((Uint8 *)surface->pixels + bounding_box->center_y * surface->pitch +
                   bounding_box->center_x * surface->format->BytesPerPixel);
        *mark_pixel = SDL_MapRGB(surface->format, MARK_COLOR_R, MARK_COLOR_G, MARK_COLOR_B);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

bool is_marked(SDL_Surface* marks_surface, ShapeBoundingBox* bounding_box){
    if (bounding_box->center_x != NULL && bounding_box->center_y != NULL){
        Uint32 *mark_pixel =
        (Uint32 *)((Uint8 *)marks_surface->pixels + bounding_box->center_y * marks_surface->pitch +
                   bounding_box->center_x * marks_surface->format->BytesPerPixel);
        Uint8 r,g,b;
        SDL_GetRGB(*mark_pixel, marks_surface->format, &r, &g, &b);
        if (r == MARK_COLOR_R && g == MARK_COLOR_G && b == MARK_COLOR_B){
            return true;
        }
        return false;
    }
    return EXIT_FAILURE;
}
/*
void letter_dfs_by_similar(SDL_Surface* marks_surface, Node* shape, linkedList* shapes, linkedList* dest, int depth){
    const int MAX_DEPTH = 3000;
    if (depth > MAX_DEPTH)
    {
        return;
    }

    int width = marks_surface->w;
    int height = marks_surface->h;

    // If pixel is already marked (blue), return
    if (is_marked(marks_surface, shape->shape_bounding_box)){
        return;
    }

    // Mark
    int x = shape->shape_bounding_box->center_x;
    int y = shape->shape_bounding_box->center_y;

    Uint32 *mark_pixel =
        (Uint32 *)((Uint8 *)marks_surface->pixels + y * marks_surface->pitch +
                   x * marks_surface->format->BytesPerPixel);
    *mark_pixel = SDL_MapRGB(marks_surface->format, MARK_COLOR_R, MARK_COLOR_G, MARK_COLOR_B);

    int dx[] = {  0,-1, 1, 0 };
    int dy[] = { -1, 0, 0, 1 };

    for (int i = 0; i < 4; i++)
    {
        int new_x = x + dx[i];
        int new_y = y + dy[i];

        if (!is_marked(marks_surface, shape->shape_bounding_box))
        {

            list_append_shape(dest, shape);
        }
    }
}

linkedList *surface_to_objects(SDL_Surface *surface)
{
    int height = surface->h;
    int width = surface->w;

    linkedList *shape_list = list_create();

    // Create a temporary surface for the result
    SDL_Surface *marks_surface = SDL_CreateRGBSurface(
        0, width, height, 32, surface->format->Rmask, surface->format->Gmask,
        surface->format->Bmask, surface->format->Amask
    );

    int sizes = 0;
    int shapes_c = 0;
*/
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