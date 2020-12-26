#ifndef ROGUELIKE_POINT_LIGHT_H
#define ROGUELIKE_POINT_LIGHT_H

#include "../../math/types.h"

typedef struct {
    size_t entity_id;
    vec3 color;
} point_light_t;

#endif //ROGUELIKE_POINT_LIGHT_H
