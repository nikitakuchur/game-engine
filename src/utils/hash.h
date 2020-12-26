#ifndef ROGUELIKE_HASH_H
#define ROGUELIKE_HASH_H

#include <stddef.h>
#include "array.h"

#define DEFAULT_TABLE_SIZE 8
#define RESIZE_FACTOR 0.75f

typedef struct hash_node {
    void *key;
    void *value;
    struct hash_node *next;
} hash_node_t;

typedef struct {
    hash_node_t **table;
    size_t table_size;
    size_t size;

    size_t (*hash)(const void *key);

    int (*key_cmp)(const void *k1, const void *k2);
} hash_t;

void hash_create(hash_t *map, size_t (*hash)(const void *key), int (*key_cmp)(const void *k1, const void *k2));

void hash_put(hash_t *map, void *key, void *value);

hash_node_t *hash_get(hash_t *map, void *key);

hash_node_t *hash_remove(hash_t *map, void *key);

void hash_get_values(hash_t *map, array_t *dest);

void hash_get_keys(hash_t *map, array_t *dest);

void hash_free(hash_t *map);

#endif //ROGUELIKE_HASH_H
