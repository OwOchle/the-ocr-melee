#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void put_pixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    // Ensure coordinates are within bounds
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32 *pixels = (Uint32 *)surface->pixels;
        pixels[y * surface->w + x] = color;
    }
}

void detect_grid_lines_y(SDL_Renderer* renderer, SDL_Surface *image, SDL_Surface *target, int line_width) {
    int width = image->w;
    int height = image->h;
    Uint32 *pixels = (Uint32 *)image->pixels;

    int sum_line_height = 0;

    for (int w = 0; w < width; w += line_width) {
        int sum_step = 0;
        for (int w2 = w; w2 < w + line_width; w2++) {
            int sum = 0;
            for (int h = 0; h < height; h++) {
                Uint32 pixel = pixels[h * width + w2];
                Uint8 r, g, b;
                SDL_GetRGB(pixel, image->format, &r, &g, &b);
                if (r != 0 || g != 0 || b != 0) {
                    sum++;
                }
            }
            sum_step += sum;
        }
        sum_line_height += (sum_step / line_width);
    }

    double avg = (double)(sum_line_height / (width / line_width));

    for (int w = 0; w < width; w += line_width) {
        int sum_step = 0;
        for (int w2 = w; w2 < w + line_width; w2++) {
            int sum = 0;
            for (int h = 0; h < height; h++) {
                Uint32 pixel = pixels[h * width + w2];
                Uint8 r, g, b;
                SDL_GetRGB(pixel, image->format, &r, &g, &b);
                if (r != 0 || g != 0 || b != 0) {
                    sum++;
                }
            }
            sum_step += sum;
        }
        if ((sum_step / line_width) >= avg) {
            // Draw the vertical line
            for (int h = 0; h < height; h++) {
                for (int x = w; x < w+line_width; x++)
                {
                    put_pixel(target, x, h, 90);
                }
                
            }
        }
    }
}

