#include "em.h"

#include <malloc.h>
#include <string.h>
#include <math.h>

static size_t id_hash(const void *key) {
    return *(size_t *) key;
}

static int id_key_cmp(const void *k1, const void *k2) {
    return *(size_t *) k1 == *(size_t *) k2;
}

static size_t name_hash(const void *key) {
    char *str = (char *) key;
    size_t n = strlen(key) + 1;
    size_t h = 0;
    for (size_t i = 0; i < n; i++) {
        h += str[i] * pow(31, (double) (n - (i + 1)));
    }
    return h;
}

static int name_key_cmp(const void *k1, const void *k2) {
    return strcmp(k1, k2) == 0;
}

void em_create() {
    array_create(&em.entities);
    hash_create(&em.components, name_hash, name_key_cmp);
}

void em_register_component(char *name) {
    size_t name_len = strlen(name) + 1;

    char *name_key = malloc(name_len * sizeof(char));
    strcpy(name_key, name);
    hash_t *hash = malloc(sizeof(hash_t));
    hash_create(hash, id_hash, id_key_cmp);
    hash_put(&em.components, name_key, hash);
}

size_t em_create_entity() {
    size_t *id = malloc(sizeof(size_t));
    *id = em.entities.size + 1;
    array_add(&em.entities, id);
    return *id;
}

void em_add_component(char *name, size_t id, component_t *component) {
    component->entity_id = id;
    hash_node_t *node = hash_get(&em.components, name);
    if (node == NULL) {
        printf("Unregistered component \"%s\"\n", name);
        return;
    }
    hash_t *component_hash = node->value;

    size_t *key = malloc(sizeof(size_t));
    *key = id;
    hash_put(component_hash, key, component);
}

void *em_get_component(char *name, size_t id) {
    hash_node_t *node = hash_get(&em.components, name);
    if (node == NULL) {
        printf("Unregistered component \"%s\"\n", name);
        return NULL;
    }

    hash_node_t *component_node = hash_get(node->value, &id);
    if (component_node == NULL) return NULL;
    return component_node->value;
}

void em_get_components(char *name, array_t *array) {
    hash_node_t *node = hash_get(&em.components, name);
    if (node == NULL) {
        printf("Unregistered component \"%s\"\n", name);
        array = NULL;
        return;
    }
    hash_get_values(node->value, array);
}

// Frees the entity manager with components
void em_free() {
    array_free(&em.entities);

    array_t component_hashes;
    array_create(&component_hashes);
    hash_get_values(&em.components, &component_hashes);
    for (size_t i = 0; i < component_hashes.size; i++) {
        hash_t* component_hash = array_get(&component_hashes, i);
        hash_free(component_hash);
        free(component_hash);
    }
    free(component_hashes.array);
}
