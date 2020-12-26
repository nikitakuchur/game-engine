#include "transform_sys.h"
#include "../../math/affine.h"
#include "../components/relationship.h"
#include "../../math/vec3.h"

void transform_to_world(transform_t *transform, transform_t *dest) {
    if (transform == NULL) return;

    vec3_zero(dest->position);
    vec3_zero(dest->rotation);
    vec3_zero(dest->scale);

    transform_t *t = transform;
    while (t != NULL) {
        vec3_add(dest->position, t->position, dest->position);
        vec3_add(dest->rotation, t->rotation, dest->rotation);
        vec3_add(dest->scale, t->scale, dest->scale);
        relationship_t *relationship = EM_GET_COMPONENT(relationship_t, t->entity_id);
        if (relationship != NULL) {
            t = EM_GET_COMPONENT(transform_t, relationship->parent);
        }
    }
}

void transform_update(transform_t *transform, mat4 model) {
    if (transform == NULL) return;

    relationship_t *relationship = EM_GET_COMPONENT(relationship_t, transform->entity_id);
    if (relationship != NULL) {
        transform_t *parent_transform = EM_GET_COMPONENT(transform_t, relationship->parent);
        transform_update(parent_transform, model);
    }

    translate(model, transform->position);
    rotate_z(model, transform->rotation[2]);
    rotate_y(model, transform->rotation[1]);
    rotate_x(model, transform->rotation[0]);
    scale(model, transform->scale);
}
