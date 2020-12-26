#ifndef ROGUELIKE_RENDER_H
#define ROGUELIKE_RENDER_H

#include <stdint.h>

typedef struct g_buffer {
    uint32_t id;

    uint32_t g_position;
    uint32_t g_normal;
    uint32_t g_albedo_spec;
} g_buffer_t;

g_buffer_t render_create_g_buffer(int scr_width, int scr_height);

void render_draw_quad();

#endif //ROGUELIKE_RENDER_H
