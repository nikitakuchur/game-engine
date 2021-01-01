#include <glad/glad.h>
#include <malloc.h>
#include "game.h"
#include "render.h"
#include "shader.h"
#include "mesh/mesh.h"
#include "primitives/plane.h"
#include "primitives/box.h"
#include "math/util.h"
#include "texture.h"
#include "ecs/em.h"
#include "ecs/components/camera.h"
#include "ecs/components/spectator.h"
#include "ecs/systems/spectator_sys.h"
#include "entities.h"
#include "ecs/components/dir_light.h"
#include "ecs/systems/dir_light_sys.h"

static GLFWwindow *game_window;
static g_buffer_t g_buffer;

// Textures
static uint32_t empty_texture;
static uint32_t tiles_texture;
static uint32_t box_texture;
static uint32_t book_fire_texture;

// Shaders
static uint32_t g_buffer_shader_program;

static uint32_t shadow_depth_shader_program;

static uint32_t amb_light_shader_program;
static uint32_t dir_light_shader_program;
static uint32_t point_light_shader_program;

void game_launch(GLFWwindow *window) {
    game_window = window;

    int scr_width, scr_height;
    glfwGetWindowSize(game_window, &scr_width, &scr_height);

    g_buffer = render_create_g_buffer(scr_width, scr_height);

    // Load textures
    empty_texture = texture_create_empty();
    tiles_texture = texture_load_rgba("../res/textures/tiles.png");
    box_texture = texture_load_rgba("../res/textures/box.png");
    book_fire_texture = texture_load_rgba("../res/textures/books/book_fire.png");

    em_create();
    EM_REGISTER_COMPONENT(transform_t);
    EM_REGISTER_COMPONENT(mesh_renderer_t);
    EM_REGISTER_COMPONENT(relationship_t);
    EM_REGISTER_COMPONENT(camera_t);
    EM_REGISTER_COMPONENT(spectator_t);
    EM_REGISTER_COMPONENT(amb_light_t);
    EM_REGISTER_COMPONENT(dir_light_t);
    EM_REGISTER_COMPONENT(point_light_t);

    create_spectator((vec3) {0.f, 1.f, 3.f});

    mesh_t plane_mesh;
    plane_create(&plane_mesh, 100.f);
    create_model(
            (vec3) {0.f, 0.f, 0.f},
            (vec3) {0.f, 0.f, 0.f},
            (vec3) {100.f, 100.f, 100.f},
            &plane_mesh,
            tiles_texture
    );

    mesh_t box_mesh;
    box_create(&box_mesh);
    create_model(
            (vec3) {2.f, 0.5f, -1.f},
            (vec3) {0.f, PI / 12.f, 0.f},
            (vec3) {1.f, 1.f, 1.f},
            &box_mesh,
            box_texture
    );

    create_model(
            (vec3) {2.03f, 1.5f, -0.5f},
            (vec3) {0.f, -(float) PI / 14.f, 0.f},
            (vec3) {1.f, 1.f, 1.f},
            &box_mesh,
            box_texture
    );

    create_model(
            (vec3) {2.f, 0.5f, 0.2f},
            (vec3) {0.f, 0.f, 0.f},
            (vec3) {1.f, 1.f, 1.f},
            &box_mesh,
            box_texture
    );

    create_model(
            (vec3) {-2.f, 0.5f, -1.6f},
            (vec3) {0.f, rad(45.f), 0.f},
            (vec3) {1.f, 1.f, 1.f},
            &box_mesh,
            box_texture
    );

    mesh_t book_mesh;
    mesh_load(&book_mesh, "../res/models/book.obj");
    create_model(
            (vec3) {2.f, 1.03f, 0.4f},
            (vec3) {0.f, rad(-40.f), 0.f},
            (vec3) {0.03f, 0.03f, 0.03f},
            &book_mesh,
            book_fire_texture
    );

    mesh_t monkey_mesh;
    mesh_load(&monkey_mesh, "../res/models/monkey_smooth.obj");
    create_model(
            (vec3) {0.f, 1.5f, 0.f},
            (vec3) {0.f, 0.f, 0.f},
            (vec3) {0.5f, 0.5f, 0.5f},
            &monkey_mesh,
            empty_texture
    );

    create_amb_light((vec3) {0.4f, 0.4f, 0.4f});
    create_dir_light((vec3) {rad(-50.f), rad(30.f), rad(0.f)}, (vec3) {0.9f, 0.9f, 0.9f});
    //create_point_light((vec3) {5.f, 5.f, 0.f}, (vec3) {1.f, 0.f, 0.f});
    //create_point_light((vec3) {-5.f, 5.f, 0.f}, (vec3) {0.f, 1.f, 0.f});
    //create_point_light((vec3) {0.f, 5.f, -5.f}, (vec3) {0.f, 0.f, 1.f});

    // Create shader programs
    g_buffer_shader_program = shader_create_program_f(
            "../res/shaders/g_buffer_vertex.glsl",
            "../res/shaders/g_buffer_fragment.glsl"
    );

    shadow_depth_shader_program = shader_create_program_f(
            "../res/shaders/shadow_depth_vertex.glsl",
            "../res/shaders/shadow_depth_fragment.glsl"
    );

    amb_light_shader_program = shader_create_program_f(
            "../res/shaders/amb_light_vertex.glsl",
            "../res/shaders/amb_light_fragment.glsl"
    );

    dir_light_shader_program = shader_create_program_f(
            "../res/shaders/dir_light_vertex.glsl",
            "../res/shaders/dir_light_fragment.glsl"
    );

    point_light_shader_program = shader_create_program_f(
            "../res/shaders/point_light_vertex.glsl",
            "../res/shaders/point_light_fragment.glsl"
    );

    // Default shaders configuration
    render_setup_shader(amb_light_shader_program);
    render_setup_shader(dir_light_shader_program);
    shader_set_int(dir_light_shader_program, "shadow_map", 3);
    render_setup_shader(point_light_shader_program);

    float last_time = (float) glfwGetTime();
    while (!glfwWindowShouldClose(game_window)) {
        float current_time = (float) glfwGetTime();
        float delta_time = current_time - last_time;
        last_time = current_time;

        game_update(delta_time);
        game_draw();

        glfwSwapBuffers(game_window);
        glfwPollEvents();
    }
    game_destroy();
}

void game_update(float dt) {
    // Input
    if (glfwGetKey(game_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(game_window, GLFW_TRUE);
    }

    array_t spectators;
    array_create(&spectators);
    EM_GET_COMPONENTS(spectator_t, &spectators);
    for (size_t i = 0; i < spectators.size; i++) {
        spectator_t *spectator = array_get(&spectators, i);
        transform_t *transform = EM_GET_COMPONENT(transform_t, spectator->entity_id);
        if (spectator != NULL && transform != NULL) {
            spectator_update(spectator, transform, game_window, dt);
        }
    }
    free(spectators.array);
}

void game_draw() {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    // 0. Render depth of scene to texture (from light's perspective)
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_draw_dir_light_shadow_maps(shadow_depth_shader_program);

    // 1. Geometry pass: render scene's geometry/color data into g-buffer
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer.id);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(g_buffer_shader_program);
    render_setup_cameras(g_buffer_shader_program, game_window);
    render_draw_meshes(g_buffer_shader_program);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. Lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the g-buffer's content.
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_bind_g_buffer_to_shader(g_buffer, amb_light_shader_program);
    render_bind_g_buffer_to_shader(g_buffer, dir_light_shader_program);
    render_bind_g_buffer_to_shader(g_buffer, point_light_shader_program);

    // Ambient lights
    render_draw_amb_lights(amb_light_shader_program);

    // Directional lights
    render_draw_dir_lights(dir_light_shader_program);

    // Point lights
    render_draw_point_lights(point_light_shader_program);
}

void game_destroy() {
    em_free();
    glDeleteProgram(g_buffer_shader_program);
    glDeleteProgram(shadow_depth_shader_program);
    glDeleteProgram(amb_light_shader_program);
    glDeleteProgram(dir_light_shader_program);
    glDeleteProgram(point_light_shader_program);
}
