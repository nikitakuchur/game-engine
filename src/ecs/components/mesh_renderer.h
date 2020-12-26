#ifndef ROGUELIKE_MESH_RENDERER_H
#define ROGUELIKE_MESH_RENDERER_H

#include <stddef.h>
#include "../../mesh/mesh.h"

typedef struct {
    size_t entity_id;
    mesh_t mesh;
    uint32_t texture;
} mesh_renderer_t;

#endif //ROGUELIKE_MESH_RENDERER_H
