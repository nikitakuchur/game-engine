#include "dir_light_sys.h"
#include "../components/transform.h"
#include "../em.h"
#include "../../shader.h"
#include "../../math/mat4.h"
#include "../../math/cam.h"
#include "../../math/affine.h"

void dir_light_draw_shadow_map(dir_light_t *dir_light, uint32_t shader, depth_map_t depth_map) {
    vec3 pos = {0.f, 0.f, 0.f};
    vec3 rot = {0.f, 0.f, 0.f};

    transform_t *transform = EM_GET_COMPONENT(transform_t, dir_light->entity_id);
    if (transform != NULL) {
        vec3_copy(pos, transform->position);
        vec3_copy(rot, transform->rotation);
    }

    mat4 light_proj = MAT4_IDENTITY_INIT;
    mat4 light_view = MAT4_IDENTITY_INIT;
    mat4 light_space_mat = MAT4_IDENTITY_INIT;

    ortho(-10.f, 10.f, -10.f, 10.f, 1.f, 7.5f, light_proj);
    rotate_z(light_view, -rot[2]);
    rotate_y(light_view, -rot[1]);
    rotate_x(light_view, -rot[0]);
    translate(light_view, pos);
    light_view[3][0] *= -1;
    light_view[3][1] *= -1;
    light_view[3][2] *= -1;

    mat4_mul(light_proj, light_view, light_space_mat);

    shader_set_mat4(shader, "light_space_mat", light_space_mat);
    mat4_copy(light_space_mat, dir_light->light_space_mat);

    glViewport(0, 0, depth_map.size, depth_map.size);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map.id);
    glClear(GL_DEPTH_BUFFER_BIT);
    render_draw_meshes(shader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void dir_light_draw(dir_light_t *dir_light, uint32_t shader) {
    vec3 dir = {0.f, 0.f, -1.f};
    transform_t *transform = EM_GET_COMPONENT(transform_t, dir_light->entity_id);
    if (transform != NULL) {
        vec3_rotate(dir, transform->rotation[2], (vec3) {0.f, 0.f, 1.f});
        vec3_rotate(dir, transform->rotation[1], (vec3) {0.f, 1.f, 0.f});
        vec3_rotate(dir, transform->rotation[0], (vec3) {1.f, 0.f, 0.f});
    }

    shader_set_mat4(shader, "light_space_mat", dir_light->light_space_mat);
    shader_set_vec3(shader, "light.dir", dir);
    shader_set_vec3(shader, "light.color", dir_light->color);

    render_draw_quad();
}
