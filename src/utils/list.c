#include "list.h"

#include <malloc.h>

void list_create(list_t *list) {
    list->size = 0;
    list->first = NULL;
    list->last = NULL;
}

void list_add(list_t *list, void *value) {
    list_node_t *n = malloc(sizeof(list_node_t));
    n->value = value;
    n->prev = NULL;
    n->next = NULL;

    if (list->last != NULL) {
        list->last->next = n;
        n->prev = list->last;
        list->last = n;
    } else {
        list->first = list->last = n;
    }

    list->size++;
}

list_node_t *list_get(list_t *list, int i) {
    int count = 0;
    for (list_node_t *n = list->first; n != NULL; n = n->next) {
        if (count == i) {
            return n;
        }
        count++;
    }
    return NULL;
}

list_node_t *list_remove_first(list_t *list) {
    list_node_t *n = list->first;
    if (n == NULL) return NULL;

    if (n->next != NULL) {
        list->first = n->next;
        list->first->prev = NULL;
    } else {
        list->first = list->last = NULL;
    }

    list->size--;
    return n;
}

list_node_t *list_remove_last(list_t *list) {
    list_node_t *n = list->last;
    if (n == NULL) return NULL;

    if (n->prev != NULL) {
        list->last = n->prev;
        list->last->next = NULL;
    } else {
        list->first = list->last = NULL;
    }

    list->size--;
    return n;
}

list_node_t *list_remove(list_t *list, int i) {
    list_node_t *n = list_get(list, i);
    if (n == NULL) return NULL;

    if (n->prev == NULL) {
        return list_remove_first(list);
    }

    if (n->next == NULL) {
        return list_remove_last(list);
    }

    n->prev->next = n->next;
    n->next->prev = n->prev;

    list->size--;
    return n;
}

void list_to_float_array(list_t *list, float **dest) {
    *dest = malloc(list->size * (sizeof(float)));

    list_node_t *n = list->first;
    for (size_t i = 0; i < list->size; i++, n = n->next) {
        (*dest)[i] = *((float *) n->value);
    }
}

void list_to_int_array(list_t *list, int **dest) {
    *dest = malloc(list->size * (sizeof(int)));

    list_node_t *n = list->first;
    for (size_t i = 0; i < list->size; i++, n = n->next) {
        (*dest)[i] = *((int *) n->value);
    }
}

// Frees the given list with values
void list_free(list_t *list) {
    for (list_node_t *n = list->first; n != NULL; n = list->first) {
        if (n->value != NULL) {
            free(n->value);
        }
        free(list_remove_first(list));
    }
}
