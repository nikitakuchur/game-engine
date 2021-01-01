#include "entities.h"

#include <stddef.h>
#include <malloc.h>
#include "ecs/em.h"
#include "ecs/components/transform.h"
#include "ecs/components/spectator.h"
#include "ecs/components/relationship.h"
#include "ecs/components/camera.h"
#include "ecs/components/mesh_renderer.h"
#include "ecs/components/amb_light.h"
#include "ecs/components/dir_light.h"
#include "ecs/components/point_light.h"
#include "math/mat4.h"

void create_spectator(vec3 position) {
    size_t spectator_id = em_create_entity();
    size_t camera_id = em_create_entity();

    // Spectator components
    transform_t *spectator_transform = malloc(sizeof(transform_t));
    vec3_copy(spectator_transform->position, position);
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

void create_model(vec3 position, vec3 rotation, vec3 scale, mesh_t *mesh, uint32_t texture) {
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

void create_amb_light(vec3 color) {
    size_t amb_light_id = em_create_entity();

    amb_light_t *amb_light = malloc(sizeof(amb_light_t));
    vec3_copy(amb_light->color, color);
    EM_ADD_COMPONENT(amb_light_t, amb_light_id, amb_light);
}

void create_dir_light(vec3 rotation, vec3 color) {
    size_t dir_light_id = em_create_entity();
    transform_t *light_transform = malloc(sizeof(transform_t));
    vec3_copy(light_transform->position, (vec3) {0.f, 6.f, 0.f});
    vec3_copy(light_transform->rotation, rotation);
    vec3_copy(light_transform->scale, (vec3) {1.f, 1.f, 1.f});
    EM_ADD_COMPONENT(transform_t, dir_light_id, light_transform);

    dir_light_t *dir_light = malloc(sizeof(dir_light_t));
    vec3_copy(dir_light->color, color);
    mat4_identity(dir_light->light_space_mat);
    dir_light->depth_map_id = 0;
    dir_light->depth_map_size = 2048;
    dir_light->depth_map_texture = 0;
    EM_ADD_COMPONENT(dir_light_t, dir_light_id, dir_light);
}

void create_point_light(vec3 position, vec3 color) {
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
