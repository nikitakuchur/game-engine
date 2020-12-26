#ifndef ROGUELIKE_TRANSFORM_SYS_H
#define ROGUELIKE_TRANSFORM_SYS_H

#include "../em.h"
#include "spectator_sys.h"

void transform_to_world(transform_t* transform, transform_t *dest);

void transform_update(transform_t* transform, mat4 model);

#endif //ROGUELIKE_TRANSFORM_SYS_H
