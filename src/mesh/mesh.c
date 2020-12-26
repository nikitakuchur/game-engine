#include "mesh.h"
#include "obj_loader.h"

#include <glad/glad.h>
#include <malloc.h>

void mesh_create(mesh_t* m, float *vertices, size_t vertices_size) {
    m->vertices_size = vertices_size;

    glGenVertexArrays(1, &m->vao);
    glGenBuffers(1, &m->vbo);

    glBindVertexArray(m->vao);

    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(float), vertices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    // Texture coords attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
}

void mesh_load(mesh_t* m, const char* filepath) {
    float *vertices = NULL;
    int vertices_size;
    obj_load(filepath, &vertices, &vertices_size);

    mesh_create(m, vertices, vertices_size);

    free(vertices);
}

void mesh_draw(mesh_t *m) {
    glBindVertexArray(m->vao);
    glDrawArrays(GL_TRIANGLES, 0, m->vertices_size / 8);
}

void mesh_delete(mesh_t *m) {
    glDeleteVertexArrays(1, &m->vao);
    glDeleteBuffers(1, &m->vbo);
    m->vertices_size = 0;
}
