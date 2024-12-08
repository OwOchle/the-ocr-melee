#include "objects_detection.h"
#include "utils/linked_list.h"
#include "utils/shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

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
    // show_shape_boundings(surface, shape, bound_color);
    //  Mark the center;
    //  printf("mark ze box\n");
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
    linkedList *dest, int depth, SDL_Color group_color, int *max_x, int *max_y,
    int *min_x, int *min_y
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

    int margin_y = 3 * shape_height;
    int margin_x = 8 * shape_width;

    // Update bounds
    *max_x = (box->max_x > *max_x) ? box->max_x : *max_x;
    *max_y = (box->max_y > *max_y) ? box->max_y : *max_y;
    *min_x = (*min_x == 0 || box->min_x < *min_x) ? box->min_x : *min_x;
    *min_y = (*min_y == 0 || box->min_y < *min_y) ? box->min_y : *min_y;

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
                        marks_surface, shape, shapes, dest, depth + 1,
                        group_color, max_x, max_y, min_x, min_y
                    );
                }
                free(box);
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
                        marks_surface, shape, shapes, dest, depth + 1,
                        group_color, max_x, max_y, min_x, min_y
                    );
                }
                free(box);
            }
        }
    }

    free(box);
}

ShapeBoundingBox **get_shape_groups(SDL_Surface *surface, linkedList *shapes, int *size)
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
    SDL_BlitSurface(surface, NULL, marks_surface, NULL);

    ShapeBoundingBox **box_res = malloc(sizeof(ShapeBoundingBox *));

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

                    int max_x = 0;
                    int max_y = 0;
                    int min_x = width;
                    int min_y = height;

                    printf("X: %i, y : %i\n", x, y);
                    SDL_Color color2 = {
                        rand() % 255, rand() % 255, rand() % 255
                    };

                    ShapeBoundingBox *box = malloc(sizeof(ShapeBoundingBox));

                    linkedList *shape_list = list_create();
                    linkedList *shape =
                        find_shape_containing_point(x, y, shapes);
                    letter_dfs_by_similar(
                        marks_surface, shape, shapes, shape_list, 0, color2,
                        &max_x, &max_y, &min_x, &min_y
                    );
                    printf("maxX: %i, maxy : %i\n", max_x, max_y);

                    int box_width = max_x - min_x;
                    int box_height = max_y - min_y;

                    if (box_width >= 0 && box_height >= 0 &&
                        max_x < surface->w && max_y < surface->h &&
                        min_y >= 0 && min_x >= 0)
                    {
                        i++;
                        show_bounding_box(
                            marks_surface, max_x, max_y, min_x, min_y, color2
                        );
                        box_res =
                            realloc(box_res, i * sizeof(ShapeBoundingBox *));
                        printf(
                            "Box %d size: Width = %d, Height = %d, Area = %d\n",
                            i, // box index
                            box_width, box_height, box_width * box_height
                        );
                        box->max_x = max_x;
                        box->max_y = max_y;
                        box->min_x = min_x;
                        box->min_y = min_y;
                        box_res[i - 1] = box;
                    }
                }
            }
        }
    }
    *size = i;
    // SDL_BlitSurface(marks_surface, NULL, surface, NULL);
    SDL_FreeSurface(marks_surface);
    return box_res;
}

void word_dfs(
    SDL_Surface *marks_surface, linkedList *shape, linkedList *shapes,
    int depth, SDL_Color group_color, int *max_x, int *max_y, int *min_x,
    int *min_y
)
{
    // Strict depth and shape validation
    const int MAX_DEPTH = 3000;
    if (depth > MAX_DEPTH || shape == NULL)
    {
        return;
    }

    ShapeBoundingBox *box = get_shape_boundings(shape);
    if (box == NULL)
    {
        return;
    }

    // Check if already marked
    if (is_marked(marks_surface, box))
    {
        free(box);
        return;
    }

    // Mark the current shape
    int x = box->center_x;
    int y = box->center_y;
    mark_box(marks_surface, box, shape, group_color);

    int shape_width = box->max_x - box->min_x;
    int shape_height = box->max_y - box->min_y;

    int margin_x = shape_width;

    // Update bounds safely
    *max_x = (box->max_x > *max_x) ? box->max_x : *max_x;
    *max_y = (box->max_y > *max_y) ? box->max_y : *max_y;
    *min_x = (*min_x == 0 || box->min_x < *min_x) ? box->min_x : *min_x;
    *min_y = (*min_y == 0 || box->min_y < *min_y) ? box->min_y : *min_y;

    // Horizontal search with strict bounds checking
    for (int i = 0; i < marks_surface->w - x - 1 && i <= margin_x; i++)
    {
        // Skip already marked pixels
        if (is_pixel_marked(marks_surface, x + i, y))
        {
            continue;
        }

        linkedList *next_shape = find_shape_containing_point(x + i, y, shapes);
        if (next_shape == NULL)
        {
            continue;
        }

        ShapeBoundingBox *next_box = get_shape_boundings(next_shape);
        if (next_box == NULL)
        {
            continue;
        }

        int next_shape_width = next_box->max_x - next_box->min_x;
        int next_shape_height = next_box->max_y - next_box->min_y;

        // Shape size compatibility check
        if (shape_height >= next_shape_height * 0.4 &&
            shape_height <= next_shape_height * 1.5)
        {
            word_dfs(
                marks_surface, next_shape, shapes, depth + 1, group_color,
                max_x, max_y, min_x, min_y
            );
        }

        free(next_box);
    }

    free(box);
}

ShapeBoundingBox **get_shape_word_groups(
    SDL_Surface *surface, linkedList *shapes, int *size,
    ShapeBoundingBox *bounds
)
{
    if (surface == NULL || shapes == NULL || size == NULL)
    {
        *size = 0;
        return NULL;
    }

    SDL_Color unmarked_color = {89, 67, 167}; // Purple (Not marked)
    show_shapes_center(surface, shapes, unmarked_color);

    int height = surface->h;
    int width = surface->w;

    // Create marks surface
    SDL_Surface *marks_surface = SDL_CreateRGBSurface(
        0, width, height, 32, surface->format->Rmask, surface->format->Gmask,
        surface->format->Bmask, surface->format->Amask
    );

    if (marks_surface == NULL)
    {
        *size = 0;
        return NULL;
    }

    SDL_BlitSurface(surface, NULL, marks_surface, NULL);

    // Initial allocation
    ShapeBoundingBox **box_res = malloc(sizeof(ShapeBoundingBox *));
    int group_count = 0;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Skip marked or non-unmarked pixels
            if (is_pixel_marked(marks_surface, x, y) ||
                !is_pixel_colored(marks_surface, x, y, unmarked_color))
            {
                continue;
            }

            // Generate a unique color for each group
            SDL_Color group_color = {
                (Uint8)(rand() % 255), (Uint8)(rand() % 255),
                (Uint8)(rand() % 255)
            };

            int max_x = 0;
            int max_y = 0;
            int min_x = width;
            int min_y = height;

            linkedList *shape = find_shape_containing_point(x, y, shapes);
            if (shape == NULL)
            {
                continue;
            }

            word_dfs(
                marks_surface, shape, shapes, 0, group_color, &max_x, &max_y,
                &min_x, &min_y
            );

            int box_width = max_x - min_x;
            int box_height = max_y - min_y;

            // Validate bounding box
            if (box_width >= 0 && box_height >= 0 && max_x < surface->w &&
                max_y < surface->h && min_y >= 0 && min_x >= 0)
            {
                group_count++;
                box_res =
                    realloc(box_res, group_count * sizeof(ShapeBoundingBox *));

                ShapeBoundingBox *new_box = malloc(sizeof(ShapeBoundingBox));
                *new_box = (ShapeBoundingBox){.min_x = min_x,
                                              .max_x = max_x,
                                              .min_y = min_y,
                                              .max_y = max_y,
                                              .center_x = (min_x + max_x) / 2,
                                              .center_y = (min_y + max_y) / 2};

                printf("%i/%i\n", bounds->max_x, bounds->max_y);
                if (max_x <= bounds->max_x && max_y <= bounds->max_y &&
                    min_x >= bounds->min_x && min_y >= bounds->min_y)
                {
                    box_res[group_count - 1] = new_box;

                    // Optional: Visualize bounding box
                    show_bounding_box(
                        marks_surface, max_x, max_y, min_x, min_y, group_color
                    );
                }
            }
        }
    }

    *size = group_count;
    SDL_BlitSurface(marks_surface, NULL, surface, NULL);
    SDL_FreeSurface(marks_surface);
    return box_res;
}