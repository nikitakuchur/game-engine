#include "amb_light_sys.h"

#include "../../shader.h"
#include "../../render.h"

void amb_light_draw(amb_light_t *amb_light, uint32_t shader) {
    shader_set_vec3(shader, "light.color", amb_light->color);
    render_draw_quad();
}
