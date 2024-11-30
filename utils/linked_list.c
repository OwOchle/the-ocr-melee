#include "linked_list.h"
#include <stdlib.h>

static Node *node_create(Pixel pixel)
{
    Node *elm = malloc(sizeof(Node));
    elm->prev = malloc(sizeof(Node *));
    elm->next = malloc(sizeof(Node *));
    elm->prev = NULL;
    elm->next = NULL;

    elm->pixel = pixel;

    return elm;
}

void list_append(linkedList *li, Pixel pixel)
{
    Node *elm = node_create(pixel);

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
