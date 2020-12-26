#include <glad/glad.h>
#include "mesh_renderer_sys.h"

void mesh_renderer_draw(mesh_renderer_t* renderer) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->texture);
    mesh_draw(&renderer->mesh);
}
