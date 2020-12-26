#ifndef ROGUELIKE_ARRAY_H
#define ROGUELIKE_ARRAY_H

#include <stddef.h>

#define DEFAULT_ARRAY_SIZE 8
#define EXPANSION_FACTOR 2

typedef struct {
    void** array;
    size_t array_size;
    size_t size;
} array_t;

void array_create(array_t* array);

void array_add(array_t* array, void* value);

void* array_get(array_t* array, size_t index);

void* array_remove(array_t* array, size_t index);

void* array_remove_last(array_t* array);

void array_free(array_t* array);

#endif //ROGUELIKE_ARRAY_H
