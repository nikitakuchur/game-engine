#ifndef ROGUELIKE_LIST_H
#define ROGUELIKE_LIST_H

#include <stddef.h>

#define LIST_INIT {NULL, NULL, 0};

typedef struct list_node {
    void *value;
    struct list_node *prev;
    struct list_node *next;
} list_node_t;

typedef struct {
    list_node_t *first;
    list_node_t *last;
    size_t size;
} list_t;

void list_create(list_t *list);

void list_add(list_t *list, void *value);

list_node_t *list_get(list_t *list, int i);

list_node_t *list_remove_first(list_t *list);

list_node_t *list_remove_last(list_t *list);

list_node_t *list_remove(list_t *list, int i);

void list_to_float_array(list_t *list, float **dest);

void list_free(list_t *list);

#endif //ROGUELIKE_LIST_H
