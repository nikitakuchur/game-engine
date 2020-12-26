#include "spectator_sys.h"

#include "../../math/vec3.h"
#include "../components/relationship.h"
#include "transform_sys.h"

static void handle_mouse(transform_t *spectator_transform, transform_t *camera_transform, GLFWwindow *window) {
    static float last_pos_x = 0.f, last_pos_y = 0.f;
    double pos_x, pos_y;
    glfwGetCursorPos(window, &pos_x, &pos_y);

    spectator_transform->rotation[1] += ((float) pos_x - last_pos_x) / 100.f;
    camera_transform->rotation[0] += ((float) pos_y - last_pos_y) / 100.f;

    last_pos_x = (float) pos_x;
    last_pos_y = (float) pos_y;
}

static void handle_keyboard(transform_t *transform, GLFWwindow *window, float speed, float dt) {
    vec3 v = {0.f, 0.f, 0.f};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3_add(v, (vec3) {0.f, 0.f, -speed * dt}, v);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3_add(v, (vec3) {-speed * dt, 0.f, 0.f}, v);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3_add(v, (vec3) {0.f, 0.f, speed * dt}, v);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3_add(v, (vec3) {speed * dt, 0.f, 0.f}, v);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        vec3_add(v, (vec3) {0.f, speed * dt, 0.f}, v);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        vec3_add(v, (vec3) {0.f, -speed * dt, 0.f}, v);
    }
    vec3_rotate(v, -transform->rotation[1], (vec3) {0.f, 1.f, 0.f});
    vec3_add(transform->position, v, transform->position);
}

void spectator_update(spectator_t *spectator, transform_t *transform, GLFWwindow *window, float dt) {
    handle_keyboard(transform, window, spectator->speed, dt);
    relationship_t *relationship = EM_GET_COMPONENT(relationship_t, transform->entity_id);
    transform_t *child_transform = EM_GET_COMPONENT(transform_t, relationship->first_child);
    handle_mouse(transform, child_transform, window);
}
