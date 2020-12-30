#ifndef ROGUELIKE_ENTITIES_H
#define ROGUELIKE_ENTITIES_H

#include <stdint.h>
#include "math/vec3.h"
#include "mesh/mesh.h"

void create_spectator(vec3 position);

void create_model(vec3 position, vec3 rotation, vec3 scale, mesh_t *mesh, uint32_t texture);

void create_amb_light(vec3 color);

void create_dir_light(vec3 rotation, vec3 color);

void create_point_light(vec3 position, vec3 color);

#endif //ROGUELIKE_ENTITIES_H
