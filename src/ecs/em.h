#ifndef ROGUELIKE_EM_H
#define ROGUELIKE_EM_H

#include "../utils/hash.h"
#include "components/component.h"

#define ENTITY_NULL 0

#define EM_REGISTER_COMPONENT(name) \
    em_register_component(#name)

#define EM_ADD_COMPONENT(name, id, component) \
    em_add_component(#name, id, (component_t *) (component))

#define EM_GET_COMPONENT(name, id) \
    em_get_component(#name, id)

#define EM_GET_COMPONENTS(name, array) \
    em_get_components(#name, array)

static struct {
    array_t entities;
    hash_t components;
} em;

void em_create();

void em_register_component(char *name);

size_t em_create_entity();

void em_add_component(char *name, size_t id, component_t *component);

void *em_get_component(char *name, size_t id);

void em_get_components(char *name, array_t *array);

void em_free();

#endif //ROGUELIKE_EM_H
