#include "objects_detection.h"
#include "utils/linked_list.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

void show_square(
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

int is_valid(SDL_Surface *surface, SDL_Surface *marks_surface, int x, int y)
{
    // printf("x: %i y: %i | R: %i, G: %i, B: %i\n", x, y, r, g, b);
    // in bounds
    if ((y >= surface->h) || (x >= surface->w) || (y < 0) || (x < 0))
    {
        return 0;
    }

    // black pixel
    Uint8 r, g, b;
    Uint32 *pixel = (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch +
                               x * surface->format->BytesPerPixel);
    SDL_GetRGB(*pixel, surface->format, &r, &g, &b);

    if (r == 255 && g == 255 && b == 255)
    {
        // printf("Invalid color : %i, %i, %i\n", r,g,b);
        return 0;
    }
    else
    {
        // printf("Valid color : %i, %i, %i\n", r,g,b);
    }

    // mark check
    Uint32 *mark_pixel =
        (Uint32 *)((Uint8 *)marks_surface->pixels + y * marks_surface->pitch +
                   x * marks_surface->format->BytesPerPixel);
    SDL_GetRGB(*pixel, marks_surface->format, &r, &g, &b);
    // r == 50 && g == 168 && b == 82
    if (!((r == 0 && g == 0 && b == 0) || (r == 255 && g == 255 && b == 255)))
    {
        printf("Marked color : %i, %i, %i\n", r, g, b);
        return 0;
    }
    // printf("Is Valid\n");
    return 1;
}

void surface_dfs(
    linkedList *list, SDL_Surface *surface, SDL_Surface *marks_surface, int x,
    int y, int depth, int mr, int mg, int mb
)
{
    const int MAX_DEPTH = 3000;
    if (depth > MAX_DEPTH)
    {
        return;
    }
    // Check if pixel is out of bounds or already processed
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h)
    {
        return;
    }

    // Check pixel color and mark status
    Uint32 *mark_pixel =
        (Uint32 *)((Uint8 *)marks_surface->pixels + y * marks_surface->pitch +
                   x * marks_surface->format->BytesPerPixel);

    // If pixel is already marked (blue), return
    Uint8 r, g, b;
    SDL_GetRGB(*mark_pixel, marks_surface->format, &r, &g, &b);
    if (!((r == 0 && g == 0 && b == 0) || (r == 255 && g == 255 && b == 255)))
    {
        return;
    }

    // Mark
    *mark_pixel = SDL_MapRGB(marks_surface->format, mr, mg, mb);

    int dx[] = {0, 1, -1, -1, 1, 0, 1, -1};
    int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};

    for (int i = 0; i < 8; i++)
    {
        int new_x = x + dx[i];
        int new_y = y + dy[i];

        if (is_valid(surface, marks_surface, new_x, new_y) == 1)
        {

            list_append(list, x, y);
            surface_dfs(
                list, surface, marks_surface, new_x, new_y, depth + 1, mr, mg,
                mb
            );
        }
    }
}

linkedList *surface_to_objects(SDL_Surface *surface)
{
    int height = surface->h;
    int width = surface->w;

    // Create a temporary surface for the result
    SDL_Surface *marks_surface = SDL_CreateRGBSurface(
        0, width, height, 32, surface->format->Rmask, surface->format->Gmask,
        surface->format->Bmask, surface->format->Amask
    );

    // Process each pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (is_valid(surface, marks_surface, x, y) == 1)
            {
                // printf("x: %i y: %i | R: %i, G: %i, B: %i\n", x, y, r, g, b);

                linkedList *list = list_create();
                list_append(list, -42, -42);
                if (x % 2 == 0)
                {
                    surface_dfs(
                        list, surface, marks_surface, x, y, 0, 168, 50, 135
                    );
                }
                else
                {
                    surface_dfs(
                        list, surface, marks_surface, x, y, 0, 36, 173, 164
                    );
                }

                if (list->tail->x != -42)
                {
                    int max_x = -1;
                    int max_y = -1;
                    int min_x = -1;
                    int min_y = -1;
                    Node *elm = list->head;
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
                    printf("(%i/%i)(%i/%i)\n", max_x, max_y, min_x, min_y);

                    SDL_Color color = {255, 255, 255};

                    if (max_x != -1)
                    {
                        show_square(
                            marks_surface, max_x, max_y, min_x, min_y, color
                        );
                    }
                }
            }
        }
    }

    // Copy back to original surface
    SDL_BlitSurface(marks_surface, NULL, surface, NULL);
    SDL_FreeSurface(marks_surface);
}
