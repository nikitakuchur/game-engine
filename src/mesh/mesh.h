#ifndef ROGUELIKE_MESH_H
#define ROGUELIKE_MESH_H

#include <stddef.h>
#include <stdint.h>

typedef struct mesh {
    uint32_t vao, vbo;
    size_t vertices_size;
} mesh_t;

void mesh_create(mesh_t *m, float *vertices, size_t vertices_size);

void mesh_load(mesh_t *m, const char *filepath);

void mesh_draw(mesh_t *m);

void mesh_delete(mesh_t *m);

#endif //ROGUELIKE_MESH_H
