#ifndef ROGUELIKE_TRANSFORM_H
#define ROGUELIKE_TRANSFORM_H

#include "../../math/types.h"

typedef struct {
    size_t entity_id;
    vec3 position;
    vec3 rotation;
    vec3 scale;
} transform_t;

#endif //ROGUELIKE_TRANSFORM_H
