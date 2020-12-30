#include "dir_light_sys.h"
#include "../components/transform.h"
#include "../em.h"
#include "../../shader.h"
#include "../../render.h"

void dir_light_draw(dir_light_t *dir_light, uint32_t shader) {
    vec3 dir = {0.f, 0.f, 1.f};
    transform_t *transform = EM_GET_COMPONENT(transform_t, dir_light->entity_id);
    if (transform != NULL) {
        vec3_rotate(dir, transform->rotation[2], (vec3) {0.f, 0.f, 1.f});
        vec3_rotate(dir, transform->rotation[1], (vec3) {0.f, 1.f, 0.f});
        vec3_rotate(dir, transform->rotation[0], (vec3) {1.f, 0.f, 0.f});
    }

    shader_set_vec3(shader, "light.dir", dir);
    shader_set_vec3(shader, "light.color", dir_light->color);

    render_draw_quad();
}
