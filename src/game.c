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
#include "ecs/systems/mesh_renderer_sys.h"
#include "math/vec3.h"
#include "ecs/systems/transform_sys.h"
#include "ecs/components/camera.h"
#include "ecs/systems/camera_sys.h"
#include "ecs/components/spectator.h"
#include "math/mat4.h"
#include "ecs/systems/spectator_sys.h"
#include "ecs/components/mesh_renderer.h"
#include "ecs/components/relationship.h"
#include "ecs/components/point_light.h"
#include "ecs/systems/point_light_sys.h"
#include "ecs/components/dir_light.h"
#include "ecs/systems/dir_light_sys.h"
#include "ecs/components/amb_light.h"
#include "ecs/systems/amb_light_sys.h"

static GLFWwindow *game_window;
static g_buffer_t g_buffer;

// Textures
static uint32_t empty_texture;
static uint32_t tiles_texture;
static uint32_t box_texture;
static uint32_t book_fire_texture;

// Shaders
static uint32_t g_buffer_shader_program;
static uint32_t amb_light_shader_program;
static uint32_t dir_light_shader_program;
static uint32_t point_light_shader_program;

static void create_spectator() {
    size_t spectator_id = em_create_entity();
    size_t camera_id = em_create_entity();

    // Spectator components
    transform_t *spectator_transform = malloc(sizeof(transform_t));
    vec3_copy(spectator_transform->position, (vec3) {0.f, 1.f, 3.f});
    vec3_copy(spectator_transform->rotation, (vec3) {0.f, 0.f, 0.f});
    vec3_copy(spectator_transform->scale, (vec3) {1.f, 1.f, 1.f});
    EM_ADD_COMPONENT(transform_t, spectator_id, spectator_transform);

    spectator_t *spectator = malloc(sizeof(spectator_t));
    spectator->speed = 2.f;
    EM_ADD_COMPONENT(spectator_t, spectator_id, spectator);

    relationship_t *spectator_relationship = malloc(sizeof(relationship_t));
    spectator_relationship->first_child = camera_id;
    spectator_relationship->next = ENTITY_NULL;
    spectator_relationship->prev = ENTITY_NULL;
    spectator_relationship->parent = ENTITY_NULL;
    EM_ADD_COMPONENT(relationship_t, spectator_id, spectator_relationship);

    // Camera components
    transform_t *camera_transform = malloc(sizeof(transform_t));
    vec3_copy(camera_transform->position, (vec3) {0.f, 0.f, 0.f});
    vec3_copy(camera_transform->rotation, (vec3) {0.f, 0.f, 0.f});
    vec3_copy(camera_transform->scale, (vec3) {1.f, 1.f, 1.0f});
    EM_ADD_COMPONENT(transform_t, camera_id, camera_transform);

    camera_t *camera = malloc(sizeof(camera_t));
    camera->active = 1;
    EM_ADD_COMPONENT(camera_t, camera_id, camera);

    relationship_t *camera_relationship = malloc(sizeof(relationship_t));
    camera_relationship->first_child = ENTITY_NULL;
    camera_relationship->next = ENTITY_NULL;
    camera_relationship->prev = ENTITY_NULL;
    camera_relationship->parent = spectator_id;
    EM_ADD_COMPONENT(relationship_t, camera_id, camera_relationship);
}

static void create_model(vec3 position, vec3 rotation, vec3 scale, mesh_t *mesh, uint32_t texture) {
    size_t model_id = em_create_entity();
    transform_t *model_transform = malloc(sizeof(transform_t));
    vec3_copy(model_transform->position, position);
    vec3_copy(model_transform->rotation, rotation);
    vec3_copy(model_transform->scale, scale);
    EM_ADD_COMPONENT(transform_t, model_id, model_transform);

    mesh_renderer_t *model_renderer = malloc(sizeof(mesh_renderer_t));
    model_renderer->mesh = *mesh;
    model_renderer->texture = texture;
    EM_ADD_COMPONENT(mesh_renderer_t, model_id, model_renderer);
}

static void create_amb_light(vec3 color) {
    size_t amb_light_id = em_create_entity();

    amb_light_t *amb_light = malloc(sizeof(amb_light_t));
    vec3_copy(amb_light->color, color);
    EM_ADD_COMPONENT(amb_light_t, amb_light_id, amb_light);
}

static void create_dir_light(vec3 rotation, vec3 color) {
    size_t dir_light_id = em_create_entity();
    transform_t *light_transform = malloc(sizeof(transform_t));
    vec3_copy(light_transform->position, (vec3) {0.f, 0.f, 0.f});
    vec3_copy(light_transform->rotation, rotation);
    vec3_copy(light_transform->scale, (vec3) {1.f, 1.f, 1.f});
    EM_ADD_COMPONENT(transform_t, dir_light_id, light_transform);

    dir_light_t *dir_light = malloc(sizeof(dir_light_t));
    vec3_copy(dir_light->color, color);
    EM_ADD_COMPONENT(dir_light_t, dir_light_id, dir_light);
}

static void create_point_light(vec3 position, vec3 color) {
    size_t point_light_id = em_create_entity();
    transform_t *light_transform = malloc(sizeof(transform_t));
    vec3_copy(light_transform->position, position);
    vec3_copy(light_transform->rotation, (vec3) {0.f, 0.f, 0.f});
    vec3_copy(light_transform->scale, (vec3) {1.f, 1.f, 1.f});
    EM_ADD_COMPONENT(transform_t, point_light_id, light_transform);

    point_light_t *point_light = malloc(sizeof(point_light_t));
    vec3_copy(point_light->color, color);
    EM_ADD_COMPONENT(point_light_t, point_light_id, point_light);
}

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

    create_spectator();

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
    mesh_load(&monkey_mesh, "../res/models/monkey.obj");
    create_model(
            (vec3) {0.f, 1.5f, 0.f},
            (vec3) {0.f, 0.f, 0.f},
            (vec3) {0.5f, 0.5f, 0.5f},
            &monkey_mesh,
            empty_texture
    );

    create_amb_light((vec3) {0.3f, 0.3f, 0.3f});
    create_dir_light((vec3) {rad(-50.f), rad(130.f), rad(0.f)}, (vec3) {0.3f, 0.3f, 0.3f});
    create_point_light((vec3) {5.f, 5.f, 0.f}, (vec3) {1.f, 0.f, 0.f});
    create_point_light((vec3) {-5.f, 5.f, 0.f}, (vec3) {0.f, 1.f, 0.f});
    create_point_light((vec3) {0.f, 5.f, -5.f}, (vec3) {0.f, 0.f, 1.f});
    //create_point_light((vec3) {0.f, 5.f, -5.f}, (vec3) {1.f, 1.f, 1.f});

    // Create shader programs
    g_buffer_shader_program = shader_create_program_f(
            "../res/shaders/g_buffer_vertex.glsl",
            "../res/shaders/g_buffer_fragment.glsl"
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

    // Default shader configurations
    glUseProgram(amb_light_shader_program);
    shader_set_int(amb_light_shader_program, "g_position", 0);
    shader_set_int(amb_light_shader_program, "g_normal", 1);
    shader_set_int(amb_light_shader_program, "g_albedo_spec", 2);

    glUseProgram(dir_light_shader_program);
    shader_set_int(dir_light_shader_program, "g_position", 0);
    shader_set_int(dir_light_shader_program, "g_normal", 1);
    shader_set_int(dir_light_shader_program, "g_albedo_spec", 2);

    glUseProgram(point_light_shader_program);
    shader_set_int(point_light_shader_program, "g_position", 0);
    shader_set_int(point_light_shader_program, "g_normal", 1);
    shader_set_int(point_light_shader_program, "g_albedo_spec", 2);

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

    // 1. Geometry pass: render scene's geometry/color data into g-buffer
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer.id);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(g_buffer_shader_program);

    // Camera setup
    array_t cameras;
    array_create(&cameras);
    EM_GET_COMPONENTS(camera_t, &cameras);
    for (size_t i = 0; i < cameras.size; i++) {
        camera_t *camera = array_get(&cameras, i);
        if (camera->active) {
            mat4 view = MAT4_IDENTITY_INIT;
            mat4 projection = MAT4_IDENTITY_INIT;

            transform_t *transform = EM_GET_COMPONENT(transform_t, camera->entity_id);
            camera_update(transform, game_window, view, projection);

            shader_set_mat4(g_buffer_shader_program, "view", view);
            shader_set_mat4(g_buffer_shader_program, "projection", projection);
            break;
        }
    }

    // Draw entities
    array_t renderers;
    array_create(&renderers);
    EM_GET_COMPONENTS(mesh_renderer_t, &renderers);
    for (size_t i = 0; i < renderers.size; i++) {
        mesh_renderer_t *renderer = array_get(&renderers, i);

        mat4 model = MAT4_IDENTITY_INIT;
        transform_t *transform = EM_GET_COMPONENT(transform_t, renderer->entity_id);
        transform_update(transform, model);

        shader_set_mat4(g_buffer_shader_program, "model", model);
        mesh_renderer_draw(renderer);
    }
    free(renderers.array);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. Lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the g-buffer's content.
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(amb_light_shader_program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_position);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_albedo_spec);

    glUseProgram(dir_light_shader_program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_position);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_albedo_spec);

    glUseProgram(point_light_shader_program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_position);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_buffer.g_albedo_spec);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Ambient lights
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


    // Directional lights
    glUseProgram(dir_light_shader_program);
    for (size_t i = 0; i < cameras.size; i++) {
        camera_t *camera = array_get(&cameras, i);
        if (camera->active) {
            transform_t *transform = EM_GET_COMPONENT(transform_t, camera->entity_id);
            if (transform != NULL) {
                transform_t world_transform;
                transform_to_world(transform, &world_transform);
                shader_set_vec3(dir_light_shader_program, "view_pos", world_transform.position);
            }
            break;
        }
    }

    // Send light relevant uniforms
    array_t dir_lights;
    array_create(&dir_lights);
    EM_GET_COMPONENTS(dir_light_t, &dir_lights);
    for (size_t i = 0; i < dir_lights.size; i++) {
        dir_light_t *dir_light = array_get(&dir_lights, i);
        dir_light_draw(dir_light, dir_light_shader_program);
    }
    free(dir_lights.array);


    // Point lights
    glUseProgram(point_light_shader_program);
    for (size_t i = 0; i < cameras.size; i++) {
        camera_t *camera = array_get(&cameras, i);
        if (camera->active) {
            transform_t *transform = EM_GET_COMPONENT(transform_t, camera->entity_id);
            if (transform != NULL) {
                transform_t world_transform;
                transform_to_world(transform, &world_transform);
                shader_set_vec3(point_light_shader_program, "view_pos", world_transform.position);
            }
            break;
        }
    }

    // Send light relevant uniforms
    array_t point_lights;
    array_create(&point_lights);
    EM_GET_COMPONENTS(point_light_t, &point_lights);
    for (size_t i = 0; i < point_lights.size; i++) {
        point_light_t *point_light = array_get(&point_lights, i);
        point_light_draw(point_light, point_light_shader_program);
    }

    free(cameras.array);
    free(point_lights.array);
}

void game_destroy() {
    em_free();
    glDeleteProgram(amb_light_shader_program);
    glDeleteProgram(dir_light_shader_program);
    glDeleteProgram(point_light_shader_program);
    glDeleteProgram(g_buffer_shader_program);
}
