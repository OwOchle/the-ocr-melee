#include "objects_detection.h"
#include "utils/linked_list.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

#include "utils/verbose.h"


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
        verbose_printf("Marked color : %i, %i, %i\n", r, g, b);
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

    linkedList *shape_list = list_create();

    // Create a temporary surface for the result
    SDL_Surface *marks_surface = SDL_CreateRGBSurface(
        0, width, height, 32, surface->format->Rmask, surface->format->Gmask,
        surface->format->Bmask, surface->format->Amask
    );

    int sizes = 0;
    int shapes_c = 0;

    // Process each pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (is_valid(surface, marks_surface, x, y) == 1)
            {
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
                // if not empty
                if (list->tail->x != -42)
                {
                    list_append_shape(shape_list, list);
                    
                    // Filtering 
                    
                }
            }
        }
    }

    // Copy back to original surface
    //SDL_BlitSurface(marks_surface, NULL, surface, NULL);
    SDL_FreeSurface(marks_surface);

    return shape_list;
}
