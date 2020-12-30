#ifndef ROGUELIKE_RENDER_H
#define ROGUELIKE_RENDER_H

#include "window.h"

#include <stdint.h>

typedef struct g_buffer {
    uint32_t id;

    uint32_t g_position;
    uint32_t g_normal;
    uint32_t g_albedo_spec;
} g_buffer_t;

g_buffer_t render_create_g_buffer(int scr_width, int scr_height);

void render_setup_shader(uint32_t shader);

void render_geometry_pass(g_buffer_t g_buffer, uint32_t g_buffer_shader_program, GLFWwindow *window);

void render_bind_g_buffer_to_shader(g_buffer_t g_buffer, uint32_t shader);

void render_setup_view_pos(uint32_t shader);

void render_draw_amb_lights(uint32_t amb_light_shader_program);

void render_draw_dir_lights(uint32_t dir_light_shader_program);

void render_draw_point_lights(uint32_t point_light_shader_program);

void render_draw_quad();

#endif //ROGUELIKE_RENDER_H
