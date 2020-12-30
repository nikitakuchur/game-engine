#ifndef ROGUELIKE_AMBIENT_LIGHT_H
#define ROGUELIKE_AMBIENT_LIGHT_H

#include <stddef.h>
#include "../../math/vec3.h"

typedef struct {
    size_t entity_id;
    vec3 color;
} amb_light_t;

#endif //ROGUELIKE_AMBIENT_LIGHT_H
