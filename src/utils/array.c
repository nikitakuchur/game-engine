#include <malloc.h>
#include "array.h"

static void fill_with_nulls(array_t *array) {
    for (size_t i = 0; i < array->array_size; i++) {
        array->array[i] = NULL;
    }
}

void array_create(array_t *array) {
    array->array = malloc(DEFAULT_ARRAY_SIZE * sizeof(void *));
    array->array_size = DEFAULT_ARRAY_SIZE;
    fill_with_nulls(array);
    array->size = 0;
}

static void expand(array_t *array) {
    void **old_array = array->array;
    size_t old_size = array->size;

    array->array_size *= EXPANSION_FACTOR;
    array->array = malloc(array->array_size * sizeof(void *));
    array->size = 0;
    fill_with_nulls(array);

    for (size_t i = 0; i < old_size; i++) {
        array_add(array, old_array[i]);
    }

    free(old_array);
}

void array_add(array_t *array, void *value) {
    if (array->size >= array->array_size) {
        expand(array);
    }
    array->array[array->size] = value;
    array->size++;
}

void* array_get(array_t *array, size_t index) {
    if (index >= array->size) {
        return NULL;
    }
    return array->array[index];
}

void* array_remove(array_t* array, size_t index) {
    if (index >= array->size) {
        return NULL;
    }
    void *removed_value = array->array[index];
    for (size_t i = index; i < array->size; i++) {
        array->array[i] = array->array[i + 1];
    }
    array->size--;
    // TODO: Add trim to size
    return removed_value;
}

void* array_remove_last(array_t* array) {
    array_remove(array, array->size - 1);
}

// Frees the given array with values
void array_free(array_t *array) {
    for (size_t i = 0; i < array->size; i++) {
        free(array->array[i]);
    }
    free(array->array);
}
