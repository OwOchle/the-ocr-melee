#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sys/types.h>
#include <unistd.h>

#include "network.h"
#include "file_io.h"
#include "../utils/array.h"
// #include "../utils/matrix.h"
// #include "../alphabet-training/read_image.h"
// #include "../alphabet-training/batch_conversion.h"
#include "evaluate.h"

#define get_pixel(surface, x, y) ((Uint8 *)surface->pixels + (y) * surface->pitch + (x) * surface->format->BytesPerPixel)

#define IMAGE_SIZE 20

#include <stdio.h>

void print_matrix(float *image, size_t size) {
    for (size_t y = 0; y < size; y++) {
        for (size_t x = 0; x < size; x++) {
            float pixel = image[y * size + x];
            if (pixel == 0.0)
                printf("\e[1;32m%.0f\e[0m", pixel);
            else 
                printf("%.0f", pixel);
        }
        printf("\n");
    }
}

float *load_image_with_SDL(const char *path)
{
    SDL_Surface *image = IMG_Load(path);

    if (image == NULL)
    {
        errx(1, "Could not load image at path '%s'", path);
    }

    float *out = malloc(sizeof(float) * IMAGE_SIZE * IMAGE_SIZE);
    for (size_t y = 0; y < image->h; y++)
    {
        for (size_t x = 0; x < image->w; x++)
        {
            Uint8 *pix = get_pixel(image, x, y);
            array_get_as_matrix(out, IMAGE_SIZE, x, y) = (pix[0] + pix[1] + pix[2]) ? 1.0 : 0.0;
        }
    }

    SDL_FreeSurface(image);

    system("clear");
    printf("Loading image representaion\n\n");
    print_matrix(out, IMAGE_SIZE);

    return out;
}

void process_image(Network *network, const char *image_path) {
    float *image = load_image_with_SDL(image_path);
    

    float *res = feedforward(network, image);
    
    char index = array_max_index_float(26, res);

    sleep(1);
    printf("____________________________________\n");
    printf("Result for %s: \e[1;31m%c\e[0m\n", image_path, 'A' + index);
    printf("____________________________________\n");

    free(image);
    free(res);
    
    sleep(4);
    
}

int main(int argc, char **argv) {
    if (argc < 3) {
        errx(1, "Usage: ./network <network path> <letter image directory>");
    }

    Network *network;
    if (network_read(&network, argv[1])) {
        errx(1, "Could not read network at path '%s'", argv[1]);
    }

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(argv[2])) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                char image_path[1024];
                snprintf(image_path, sizeof(image_path), "%s/%s", argv[2], ent->d_name);
                process_image(network, image_path);
            }
        }
        closedir(dir);
    } else {
        errx(1, "Could not open directory '%s'", argv[2]);
    }

    // size_t count;
    // InputImage *output = load_directory(argv[2], &count);
    // if (output == NULL) {
    //     errx(2, "output is null");
    // }

    // printf("Evaluating network...\n");
    // Batch *batch = images_to_batch(count, output);

    // // print_matrix(batch->layers[2]->outputData, IMAGE_SIZE);
    // // print_matrix(batch->layers[1]->outputData, IMAGE_SIZE);


    // int acc = accuracy(network, batch);

    // printf("Accuracy: %d/%zu\n", acc, count);

    // free(output);
    // batch_free(batch);
    // network_free(network);
    return 0;
}