#include "point_light_sys.h"

#include "../../shader.h"
#include "../em.h"
#include "../components/transform.h"
#include "../../math/vec3.h"
#include "../../render.h"

void point_light_draw(point_light_t *point_light, uint32_t shader) {
    vec3 pos = {0.f, 0.f, 0.f};
    transform_t *transform = EM_GET_COMPONENT(transform_t, point_light->entity_id);
    if (transform != NULL) {
        vec3_copy(pos, transform->position);
    }

    shader_set_vec3(shader, "light.pos", pos);
    shader_set_vec3(shader, "light.color", point_light->color);
    // Update attenuation parameters and calculate radius
    const float linear = 0.7f / 100;
    const float quadratic = 1.8f / 100;
    shader_set_float(shader, "light.linear", linear);
    shader_set_float(shader, "light.quadratic", quadratic);

    render_draw_quad();
}
