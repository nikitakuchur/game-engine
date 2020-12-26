#include "camera_sys.h"
#include "../../math/util.h"
#include "../../math/cam.h"
#include "../../math/affine.h"
#include "transform_sys.h"
#include "../components/relationship.h"

static void camera_view_update(transform_t* transform, mat4 view) {
    if (transform == NULL) {
        return;
    }
    rotate_z(view, transform->rotation[2]);
    rotate_y(view, transform->rotation[1]);
    rotate_x(view, transform->rotation[0]);
    translate(view, transform->position);
    view[3][0] *= -1;
    view[3][1] *= -1;
    view[3][2] *= -1;

    relationship_t *relationship = EM_GET_COMPONENT(relationship_t, transform->entity_id);
    if (relationship != NULL) {
        transform_t *parent_transform = EM_GET_COMPONENT(transform_t, relationship->parent);
        camera_view_update(parent_transform, view);
    }
}

void camera_update(transform_t* transform, GLFWwindow *window, mat4 view, mat4 projection) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    perspective(rad(60.f), (float) width / (float) height, 0.1f, 1000.f, projection);
    if (transform != NULL) {
        camera_view_update(transform, view);
    }
}
