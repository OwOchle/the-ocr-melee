#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Node Node;
typedef struct Node Node_Shape;
typedef struct Pixel Pixel;
typedef struct linkedList linkedList;

struct ShapeBoundingBox;

struct Pixel
{
    int x;
    int y;
    Uint32 *pixel;
};

struct Node
{
    Node *prev;
    Node *next;
    int x;
    int y;
    linkedList *shape;
    struct ShapeBoundingBox* shape_bounding_box;
};

struct linkedList
{
    Node *head;
    Node *tail;
};

static Node *node_create(int x, int y);
static Node_Shape *node_create_shape(linkedList* shape);

void list_append(linkedList *li, int x, int y);
void list_append_shape(linkedList *li, linkedList* shape);

linkedList *list_create();

void list_free(linkedList *li);
void list_free_shape(linkedList *li);

long get_shape_pixel_count(linkedList* shape);

size_t list_length(linkedList *li);