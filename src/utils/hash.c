#include <malloc.h>
#include "hash.h"
#include "array.h"

static void fill_with_nulls(hash_t *hash) {
    for (int i = 0; i < hash->table_size; i++) {
        hash->table[i] = NULL;
    }
}

void hash_create(hash_t* map, size_t (*hash)(const void *), int (*key_cmp)(const void *, const void *)) {
    map->table = malloc(DEFAULT_TABLE_SIZE * sizeof(hash_node_t *));
    map->table_size = DEFAULT_TABLE_SIZE;
    fill_with_nulls(map);
    map->size = 0;
    map->hash = hash;
    map->key_cmp = key_cmp;
}

size_t compute_index(hash_t *map, void *key) {
    size_t hash_value = (*map->hash)(key);
    return hash_value % map->table_size;
}

static void expand(hash_t *map) {
    hash_node_t **old_table = map->table;
    size_t old_table_size = map->table_size;

    map->table_size *= 2;
    map->table = malloc(map->table_size * sizeof(hash_node_t *));
    fill_with_nulls(map);

    for (size_t i = 0; i < old_table_size; i++) {
        hash_node_t *node = old_table[i];
        while (node != NULL) {
            hash_put(map, node->key, node->value);
            hash_node_t *removed_node = node;
            node = node->next;
            free(removed_node);
        }
    }

    free(old_table);
}

void hash_put(hash_t *map, void *key, void *value) {
    if (value == NULL) return;

    if (map->size + 1 > (size_t) (RESIZE_FACTOR * map->table_size)) {
        expand(map);
    }

    size_t index = compute_index(map, key);

    hash_node_t **node = &map->table[index];
    while (*node != NULL && !(*map->key_cmp)((*node)->key, key)) {
        node = &(*node)->next;
    }
    if (*node == NULL) {
        *node = malloc(sizeof(hash_node_t));
        (*node)->next = NULL;
    }
    (*node)->key = key;
    (*node)->value = value;

    map->size++;
}

hash_node_t *hash_get(hash_t *map, void *key) {
    size_t index = compute_index(map, key);

    hash_node_t **node = &map->table[index];
    while (*node != NULL && !(*map->key_cmp)((*node)->key, key)) {
        node = &(*node)->next;
    }
    return *node;
}

hash_node_t *hash_remove(hash_t *map, void *key) {
    size_t index = compute_index(map, key);

    hash_node_t **node = &map->table[index];
    while (*node != NULL && !(*map->key_cmp)((*node)->key, key)) {
        node = &(*node)->next;
    }
    if (*node == NULL) {
        return NULL;
    }
    hash_node_t *removed_node = *node;
    *node = (*node)->next;
    // TODO: Add trim to size
    return removed_node;
}

void hash_get_keys(hash_t *map, array_t *dest) {
    for (size_t i = 0; i < map->table_size; i++) {
        hash_node_t *node = map->table[i];
        while (node != NULL) {
            array_add(dest, node->key);
            node = node->next;
        }
    }
}

void hash_get_values(hash_t *map, array_t *dest) {
    for (size_t i = 0; i < map->table_size; i++) {
        hash_node_t *node = map->table[i];
        while (node != NULL) {
            array_add(dest, node->value);
            node = node->next;
        }
    }
}

// Frees the given hash with keys and values
void hash_free(hash_t *map) {
    for (size_t i = 0; i < map->table_size; i++) {
        hash_node_t **node = &map->table[i];
        while (*node != NULL) {
            hash_node_t *removed_node = *node;
            *node = (*node)->next;
            free(removed_node->key);
            free(removed_node->value);
            free(removed_node);
        }
    }
    free(map->table);
}
