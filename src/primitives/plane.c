#include "plane.h"

void plane_create(mesh_t *m, float tex_scale) {
    float vertices[] = {
            -0.5f, 0.f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, tex_scale,
            0.5f, 0.f, 0.5f, 0.0f, 1.0f, 0.0f, tex_scale, 0.0f,
            0.5f, 0.f, -0.5f, 0.0f, 1.0f, 0.0f, tex_scale, tex_scale,
            0.5f, 0.f, 0.5f, 0.0f, 1.0f, 0.0f, tex_scale, 0.0f,
            -0.5f, 0.f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, tex_scale,
            -0.5f, 0.f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };
    mesh_create(m, vertices, 48);
}
