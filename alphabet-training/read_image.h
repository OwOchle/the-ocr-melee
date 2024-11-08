#pragma once
#include <stdint.h>

#define IMAGE_SIZE 25
#define uchar unsigned char

uchar *load_image(char *path);

uchar **load_directory(char *path);