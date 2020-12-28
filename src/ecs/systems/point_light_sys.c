#include "point_light_sys.h"

#include "../../shader.h"
#include "../em.h"
#include "../components/transform.h"
#include "../../math/vec3.h"
#include "../../render.h"

void point_light_draw(point_light_t *point_light, uint32_t shader) {
    vec3 pos = {0.f, 0.f, 0.f};
    transform_t *transform = EM_GET_COMPONENT(transform_t,  point_light->entity_id);
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
    // then calculate radius of light volume/sphere
    //const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
    //float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
    //shader_set_float(shader, "lights[" + std::to_string(i) + "].Radius", radius);
    render_draw_quad();
}

