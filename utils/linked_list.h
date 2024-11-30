#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Node Node;
typedef struct Pixel Pixel;
typedef struct linkedList linkedList;

struct Pixel
{
    int x;
    int y;
    Uint32 pixel;
};

struct Node
{
    Node *prev;
    Node *next;
    Pixel pixel;
};

struct linkedList
{
    Node *head;
    Node *tail;
};

static Node *node_create(Pixel pixel);

void list_append(linkedList *li, Pixel pixel);

linkedList *list_create();

void list_free(linkedList *li);
