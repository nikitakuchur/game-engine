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

typedef struct depth_map {
    uint32_t id;

    uint32_t size;
    uint32_t texture;
} depth_map_t;

g_buffer_t render_create_g_buffer(int scr_width, int scr_height);

void render_setup_shader(uint32_t shader);

void render_setup_cameras(uint32_t shader, GLFWwindow *window);

void render_draw_meshes(uint32_t shader);

void render_bind_g_buffer_to_shader(g_buffer_t g_buffer, uint32_t shader);

void render_draw_amb_lights(uint32_t amb_light_shader_program);

void render_draw_dir_light_shadow_maps(uint32_t shadow_depth_shader_program);

void render_draw_dir_lights(uint32_t dir_light_shader_program);

void render_draw_point_lights(uint32_t point_light_shader_program);

void render_draw_quad();

#endif //ROGUELIKE_RENDER_H
