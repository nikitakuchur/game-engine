#include "render.h"

#include "shader.h"
#include "utils/array.h"
#include "ecs/em.h"
#include "ecs/components/camera.h"
#include "math/mat4.h"
#include "ecs/components/transform.h"
#include "ecs/systems/camera_sys.h"
#include "ecs/components/mesh_renderer.h"
#include "ecs/systems/transform_sys.h"
#include "ecs/systems/mesh_renderer_sys.h"
#include "ecs/components/amb_light.h"
#include "ecs/systems/amb_light_sys.h"
#include "ecs/components/dir_light.h"
#include "ecs/systems/dir_light_sys.h"
#include "ecs/components/point_light.h"
#include "ecs/systems/point_light_sys.h"

#include <glad/glad.h>
#include <stdio.h>
#include <malloc.h>

g_buffer_t render_create_g_buffer(int scr_width, int scr_height) {
    g_buffer_t g_buffer;

    // Configure g-buffer framebuffer
    glGenFramebuffers(1, &g_buffer.id);
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer.id);

    // Position color buffer
    glGenTextures(1, &g_buffer.g_position);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scr_width, scr_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_buffer.g_position, 0);

    // Normal buffer
    glGenTextures(1, &g_buffer.g_normal);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scr_width, scr_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_buffer.g_normal, 0);

    // Color + specular color buffer
    glGenTextures(1, &g_buffer.g_albedo_spec);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_albedo_spec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scr_width, scr_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_buffer.g_albedo_spec, 0);

    // Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    uint32_t attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    // Create and attach depth buffer (renderbuffer)
    uint32_t rbo_depth;
    glGenRenderbuffers(1, &rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scr_width, scr_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);

    // Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return g_buffer;
}

void render_setup_shader(uint32_t shader) {
    glUseProgram(shader);
    shader_set_int(shader, "g_position", 0);
    shader_set_int(shader, "g_normal", 1);
    shader_set_int(shader, "g_albedo_spec", 2);
}

depth_map_t render_create_depth_map(uint32_t size) {
    depth_map_t depth_map;
    depth_map.size = size;
    glGenFramebuffers(1, &depth_map.id);

    // Create depth texture
    glGenTextures(1, &depth_map.texture);
    glBindTexture(GL_TEXTURE_2D, depth_map.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depth_map.size, depth_map.size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach depth texture
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map.id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map.texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return depth_map;
}

void render_setup_cameras(uint32_t shader, GLFWwindow *window) {
    array_t cameras;
    array_create(&cameras);
    EM_GET_COMPONENTS(camera_t, &cameras);
    for (size_t i = 0; i < cameras.size; i++) {
        camera_t *camera = array_get(&cameras, i);
        if (camera->active) {
            mat4 view = MAT4_IDENTITY_INIT;
            mat4 projection = MAT4_IDENTITY_INIT;

            transform_t *transform = EM_GET_COMPONENT(transform_t, camera->entity_id);
            camera_update(transform, window, view, projection);

            shader_set_mat4(shader, "view", view);
            shader_set_mat4(shader, "projection", projection);
            break;
        }
    }
    free(cameras.array);
}

void render_draw_meshes(uint32_t shader) {
    array_t renderers;
    array_create(&renderers);
    EM_GET_COMPONENTS(mesh_renderer_t, &renderers);
    for (size_t i = 0; i < renderers.size; i++) {
        mesh_renderer_t *renderer = array_get(&renderers, i);

        mat4 model = MAT4_IDENTITY_INIT;
        transform_t *transform = EM_GET_COMPONENT(transform_t, renderer->entity_id);
        transform_update(transform, model);

        shader_set_mat4(shader, "model", model);
        mesh_renderer_draw(renderer);
    }
    free(renderers.array);
}

void render_bind_g_buffer_to_shader(g_buffer_t g_buffer, uint32_t shader) {
    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_position);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_albedo_spec);
}

void render_setup_view_pos(uint32_t shader) {
    array_t cameras;
    array_create(&cameras);
    EM_GET_COMPONENTS(camera_t, &cameras);

    glUseProgram(shader);
    for (size_t i = 0; i < cameras.size; i++) {
        camera_t *camera = array_get(&cameras, i);
        if (camera->active) {
            transform_t *transform = EM_GET_COMPONENT(transform_t, camera->entity_id);
            if (transform != NULL) {
                transform_t world_transform;
                transform_to_world(transform, &world_transform);
                shader_set_vec3(shader, "view_pos", world_transform.position);
            }
            break;
        }
    }

    free(cameras.array);
}

void render_draw_amb_lights(uint32_t amb_light_shader_program) {
    glUseProgram(amb_light_shader_program);

    // Send light relevant uniforms
    array_t amb_lights;
    array_create(&amb_lights);
    EM_GET_COMPONENTS(amb_light_t, &amb_lights);
    for (size_t i = 0; i < amb_lights.size; i++) {
        amb_light_t *amb_light = array_get(&amb_lights, i);
        amb_light_draw(amb_light, amb_light_shader_program);
    }
    free(amb_lights.array);
}

void render_draw_dir_lights(uint32_t dir_light_shader_program) {
    glUseProgram(dir_light_shader_program);

    array_t dir_lights;
    array_create(&dir_lights);
    EM_GET_COMPONENTS(dir_light_t, &dir_lights);
    for (size_t i = 0; i < dir_lights.size; i++) {
        dir_light_t *dir_light = array_get(&dir_lights, i);
        dir_light_draw(dir_light, dir_light_shader_program);
    }
    free(dir_lights.array);
}

void render_draw_point_lights(uint32_t point_light_shader_program) {
    glUseProgram(point_light_shader_program);

    array_t point_lights;
    array_create(&point_lights);
    EM_GET_COMPONENTS(point_light_t, &point_lights);
    for (size_t i = 0; i < point_lights.size; i++) {
        point_light_t *point_light = array_get(&point_lights, i);
        point_light_draw(point_light, point_light_shader_program);
    }
    free(point_lights.array);
}

void render_draw_quad() {
    static uint32_t quad_vao = 0;
    static uint32_t quad_vbo;

    if (quad_vao == 0) {
        float quad_vertices[] = {
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quad_vao);
        glGenBuffers(1, &quad_vbo);
        glBindVertexArray(quad_vao);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    }
    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
