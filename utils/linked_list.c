#include "linked_list.h"
#include <stdlib.h>

static Node *node_create(int x, int y)
{
    Node *elm = malloc(sizeof(Node));
    elm->prev = malloc(sizeof(Node *));
    elm->next = malloc(sizeof(Node *));
    elm->prev = NULL;
    elm->next = NULL;

    elm->x = x;
    elm->y = y;

    return elm;
}

static Node_Shape *node_create_shape(linkedList* shape)
{
    Node_Shape *elm = malloc(sizeof(Node_Shape));
    elm->prev = malloc(sizeof(Node_Shape *));
    elm->next = malloc(sizeof(Node_Shape *));
    elm->prev = NULL;
    elm->next = NULL;

    elm->shape = shape;

    return elm;
}

void list_append(linkedList *li, int x, int y)
{
    Node *elm = node_create(x, y);

    if (li->head == NULL && li->tail == NULL)
    { // Empty list
        li->head = elm;
        li->tail = elm;
    }
    else
    {
        Node *tailNode = li->tail;
        elm->next = NULL;
        elm->prev = tailNode;

        tailNode->next = elm;
        li->tail = elm;
    }
}

void list_append_shape(linkedList *li, linkedList* shape){

    Node *elm = node_create_shape(shape);

    if (li->head == NULL && li->tail == NULL)
    { // Empty list
        li->head = elm;
        li->tail = elm;
    }
    else
    {
        Node *tailNode = li->tail;
        elm->next = NULL;
        elm->prev = tailNode;

        tailNode->next = elm;
        li->tail = elm;
    }
}



linkedList *list_create()
{
    linkedList *li = malloc(sizeof(linkedList));

    li->head = malloc(sizeof(linkedList *));
    li->tail = malloc(sizeof(linkedList *));
    li->head = NULL;
    li->tail = NULL;

    return li;
}

void list_free(linkedList *li)
{
    while (li->head != NULL)
    {
        Node *elm = li->head;

        li->head = li->head->next;

        free(elm);
    }

    free(li->head);
    free(li);
}

void list_free_shape(linkedList *li)
{
    while (li->head != NULL)
    {
        Node *elm = li->head;

        li->head = li->head->next;

        list_free(elm->shape);

        free(elm);
    }

    free(li->head);
    free(li);
}

long get_shape_pixel_count(linkedList* shape){
    long i = 0;
    Node *elm = shape->head;

    while (elm->next != NULL)
    {
        if (elm->x != -42 && elm->y != -42)
        {
            i++;   
        }
        elm = elm->next;
    }
    return i;
}