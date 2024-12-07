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

int round_to_upper_five_multiple(int num) {
    int remainder = num % 5;
    if (remainder < 3) {
        return num - remainder;
    } else {
        return num + (5 - remainder);
    }
}

void show_shape_boundings(SDL_Surface *surface, linkedList* shape, SDL_Color color)
{
    ShapeBoundingBox *box = get_shape_boundings(shape);

    show_bounding_box(surface, box->max_x, box->max_y, box->min_x, box->min_y, color);

    
    show_bounding_box(surface, round_to_upper_five_multiple(box->center_x), round_to_upper_five_multiple(box->center_y), round_to_upper_five_multiple(box->center_x), round_to_upper_five_multiple(box->center_y), color);
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

    int x, y;
    get_shape_center(&x, &y, shape_box);
    shape_box->center_x = x;
    shape_box->center_y = y;

    return shape_box;
}

void get_shape_center(int* x, int* y, ShapeBoundingBox* shape_boundings){
    *x = (shape_boundings->max_x + shape_boundings->min_x)/2;
    *y = (shape_boundings->max_y + shape_boundings->min_y)/2;
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

    bool status = is_in_shape_bounds(x,y, shape_boundings);
    return status;
}

linkedList* find_shape_containing_point(int x, int y, linkedList* shapes) {
    if (shapes == NULL || shapes->head == NULL) {
        return NULL;
    }

    Node* current = shapes->head;
    while (current != NULL) {
        // Skip nodes with special marker coordinates
        if (current->x != -42 && current->y != -42) {
            // Ensure shape_bounding_box is created if not already exists
            if (current->shape_bounding_box == NULL) {
                current->shape_bounding_box = get_shape_boundings(current->shape);
            }

            // First, check if point is within bounding box
            if (is_in_shape_bounds(x, y, current->shape_bounding_box)) {
                // If in bounds, do more precise shape checking if needed
                if (is_in_shape(x, y, current->shape)) {
                    return current->shape;
                }
            }
        }
        current = current->next;
    }

    return NULL;
}


linkedList* detect_unique_shapes(linkedList* shapes) {
    linkedList* unique_shapes = list_create();

    Node* current = shapes->head;
    while (current != NULL) {
        // Skip invalid shapes
        if (current->x == -42 && current->y == -42) {
            current = current->next;
            continue;
        }

        // Ensure bounding box is created
        current->shape_bounding_box = get_shape_boundings(current->shape);
        
        // Get current shape's characteristics
        int current_width = current->shape_bounding_box->max_x - current->shape_bounding_box->min_x;
        int current_height = current->shape_bounding_box->max_y - current->shape_bounding_box->min_y;
        int current_y_mid = current->shape_bounding_box->center_y;
        int current_x_mid = current->shape_bounding_box->center_x;

        bool is_I_like = current_height > 4*current_width;

        // Flag to track if a similar shape is found
        bool similar_shape_found = false;

        // Compare with other shapes
        Node* compare = shapes->head;
        while (compare != NULL) {
            // Skip comparing shape with itself or invalid shapes
            if (compare == current || compare->x == -42 && compare->y == -42) {
                compare = compare->next;
                continue;
            }

            // Ensure comparison shape's bounding box is created
            compare->shape_bounding_box = get_shape_boundings(compare->shape);

            // Check vertical proximity (same line)
            int compare_y_mid = compare->shape_bounding_box->center_y;
            int y_diff = abs(current_y_mid - compare_y_mid);

            // Check horizontal separation
            int compare_x_mid = compare->shape_bounding_box->center_x;
            int x_diff = abs(current_x_mid - compare_x_mid);

            // Check shape size similarity
            int compare_width = compare->shape_bounding_box->max_x - compare->shape_bounding_box->min_x;
            int compare_height = compare->shape_bounding_box->max_y - compare->shape_bounding_box->min_y;

            // Conditions for considering shapes as similar:
            // 1. Vertical proximity (within height of a shape)
            // 2. Horizontal separation (less than 2x the average width)
            // 3. Similar width (40-160% of each other)
            // 4. Similar height (40-160% of each other)
            bool vertical_proximity = y_diff <= current_height * 2;
            bool horizontal_proximity;
            if (is_I_like){
                horizontal_proximity = x_diff <= (current_width + compare_width) * 12;
            }
            else {
                horizontal_proximity = x_diff <= (current_width + compare_width) * 2;
            }

            bool size_similarity;
            if (is_I_like){
                size_similarity = 
                (current_height >= compare_height * 0.4 && current_height <= compare_height * 1.6);
            }
            else
            {
                size_similarity = 
                (current_width >= compare_width * 0.4 && current_width <= compare_width * 1.8) &&
                (current_height >= compare_height * 0.4 && current_height <= compare_height * 1.6);
            }
            

            if (vertical_proximity && horizontal_proximity && size_similarity) {
                similar_shape_found = true;
                break;
            }

            compare = compare->next;
        }

        // If no similar shape found, add to unique shapes
        if (similar_shape_found) {
            list_append_shape(unique_shapes, current->shape);
        }

        current = current->next;
    }

    return unique_shapes;
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