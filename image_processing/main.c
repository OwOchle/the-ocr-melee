#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <string.h>

#include "grayscale.h"
#include "gaussian_binary.h"
#include "objects_detection.h"
#include "utils/linked_list.h"
#include "utils/shapes.h"
#include "letter_filtering.h"
#include "grid_detection.h"
#include "sobel.h"
#include "cutter.h"

#define FP_SIZE 2097152
#define BUF_SIZE 2048

struct output {
    size_t width;
    size_t height;
    size_t word_count;
    size_t word_lengths[32];
};

void execute_full(SDL_Surface *input, SDL_Surface *clean_input, char *output_path)
{
    surface_to_grayscale(input);
    surface_to_grayscale(clean_input);

    surface_to_simple_binary(input, 210);
    surface_to_simple_binary(clean_input, 210);

    linkedList *shapes = surface_to_objects(input);

    linkedList *filtered_shapes = filter_shapes(shapes);

    list_free(detect_unique_shapes(filtered_shapes));

    int box_count;
    ShapeBoundingBox **boxes = get_shape_groups(input, filtered_shapes, &box_count);

    ShapeBoundingBox *grid_box = get_largest_under(boxes, box_count, (size_t) - 1);

    ShapeBoundingBox *words_box = get_largest_under(boxes, box_count, sbb_area(grid_box));

    int word_count;
    ShapeBoundingBox **words = get_shape_word_groups(
        input, filtered_shapes, &word_count, words_box
    );

    linkedList *letters;
    size_t letter_index;
    ShapeBoundingBox *word_bb;

    char *letter_name;
    SDL_Surface *letter;
    ShapeBoundingBox *letter_bb;

    size_t *word_lengths = malloc(sizeof(size_t) * word_count);

    // Cutting words
    for (size_t i = 0; i < word_count; i++)
    {
        letter_index = 0;
        
        word_bb = words[i];

        letters = find_shapes_in_boundings(filtered_shapes, word_bb);

        for (Node *n = letters->head; n; n = n->next)
        {
            asprintf(&letter_name, "%s/word_%d_letter_%d.png", output_path, i, letter_index);
            letter_bb = get_shape_boundings(n->shape);

            letter = split_bb(clean_input, letter_bb);

            size_t w = letter->w;
            size_t h = letter->h;

            SDL_Surface *out_surface = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);

            SDL_Rect rect = { 0, 0, 20, 20 };

            SDL_FillRect(out_surface, &rect, 0xFFFFFFFF);

            SDL_Rect in_rect = {0,0,w,h};
            SDL_Rect out_rect = {1,1,18,18};

            SDL_BlitScaled(letter, &in_rect, out_surface, &out_rect);

            IMG_SavePNG(out_surface, letter_name);

            free(letter_name);
            free(letter_bb);
            SDL_FreeSurface(letter);
            letter_index++;
        }

        list_free(letters);

        word_lengths[i] = letter_index;
    }

    linkedList *letters_in_grid = find_shapes_in_boundings(filtered_shapes, grid_box);

    size_t cur_line = 0;
    size_t cur_col = 0;
    
    size_t max_col = 0;

    Node *top_left = find_top_left(letters_in_grid);

    ShapeBoundingBox *line_left = top_left->shape_bounding_box;
    ShapeBoundingBox *l = top_left->shape_bounding_box;

    SDL_Surface *cur_grid_letter;
    char *name;

    do 
    {
        l = line_left;
        do 
        {
            asprintf(&name, "%s/grid_%lu_%lu.png", output_path, cur_col, cur_line);
            cur_grid_letter = split_bb(clean_input, l);
            size_t w = cur_grid_letter->w;
            size_t h = cur_grid_letter->h;

            SDL_Surface *out_surface = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);

            SDL_Rect rect = { 0, 0, 20, 20 };

            SDL_FillRect(out_surface, &rect, 0xFFFFFFFF);

            SDL_Rect in_rect = {0,0,w,h};
            SDL_Rect out_rect = {1,1,18,18};

            SDL_BlitScaled(cur_grid_letter, &in_rect, out_surface, &out_rect);

            IMG_SavePNG(out_surface, name);

            free(name);
            SDL_FreeSurface(cur_grid_letter);
            cur_col++;
        } while ((l = find_nearest_right(letters_in_grid, l)) != NULL);

        if (cur_col > max_col)
            max_col = cur_col;
        
        cur_line++;
        cur_col = 0;
    } while ((line_left = find_nearest_down(letters_in_grid, line_left)) != NULL);

    struct output dat;

    dat.width = max_col;
    dat.height = cur_line;
    dat.word_count = word_count;

    for (size_t i = 0; i < word_count; i++)
    {
        dat.word_lengths[i] = word_lengths[i];
    }

    fwrite(&dat, sizeof(struct output), 1, stdout);
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr,
        "Usage: ./image_processing <input image> <step> <output>\n"
        "       Arguments:\n"
        "           input image: Input image path\n"
        "           step: 'grayscale', 'binary', 'shapes', 'sobel'\n"
        "           output: Output image path or '-' for stdout\n"
        "\n"
        "       Notes:\n"
        "           if step is 'full', output must be a path to a directory.\n"
        );
        exit(1);
    }

    SDL_Surface *input_image = IMG_Load(argv[1]);
    SDL_Surface *clean_surface = IMG_Load(argv[1]);

    if (input_image == NULL)
    {
        errx(1, "Error while loading input image: %s", SDL_GetError());
    }

    if (!strcmp(argv[2], "grayscale"))
    {
        surface_to_grayscale(input_image);
    } 
    else if (!strcmp(argv[2], "binary"))
    {
        surface_to_grayscale(input_image);
        surface_to_simple_binary(input_image, 128);
    }
    else if (!strcmp(argv[2], "shapes"))
    {
        surface_to_grayscale(input_image);
        surface_to_simple_binary(input_image, 128);

        linkedList* shapes = surface_to_objects(input_image);
        linkedList* filtered_shapes = filter_shapes(shapes);

        SDL_Color color = {89, 67, 167};
        linkedList* isolated = detect_unique_shapes(filtered_shapes);

        show_shapes_boundings(input_image, isolated, color);
        
        list_free(isolated);
        list_free(filtered_shapes);
    }
    else if (!strcmp(argv[2], "sobel"))
    {
        float* gradient_magnitude = malloc(input_image->w * input_image->h * sizeof(float));
        float* gradient_direction = malloc(input_image->w * input_image->h * sizeof(float));

        surface_to_grayscale(input_image);
        surface_to_simple_binary(input_image, 128);
        surface_to_sobel(input_image, gradient_magnitude, gradient_direction);

        free(gradient_direction);
        free(gradient_magnitude);
    }
    else if (!strcmp(argv[2], "full"))
    {
        execute_full(input_image, clean_surface, argv[3]);
    }
    else
    {
        errx(1, "Unknown step '%s'. Possible values are: grayscale, binary, shapes, sobel, full", argv[2]);
    }

    if (!strcmp(argv[3], "-"))
    {
        SDL_RWops *ops = SDL_RWFromFP(stdout, 1);

        if (IMG_SavePNG_RW(input_image, ops, 0))
        {
            errx(1, "Error while writing image: %s", SDL_GetError());
        }

        SDL_FreeRW(ops);
    }
    else
    {
        IMG_SavePNG(input_image, argv[3]);
    }

    SDL_FreeSurface(input_image);
}
