#pragma once
#include <stdint.h>
#include <stdlib.h>

#define IMAGE_SIZE 20
#define uchar unsigned char

typedef enum imgcat_t {
    IMG_PLAIN = 1,
    IMG_OFFCENTER,
    IMG_ROTATED
} imgcat_t;

typedef struct InputImage
{
    imgcat_t category;
    char character;
    uchar image[IMAGE_SIZE * IMAGE_SIZE];
    char name[256];
} InputImage;

InputImage *load_directory(char *path, size_t *count);

int load_image(char *path, InputImage *output);

int load_image_SDL(const char *path, InputImage *output);