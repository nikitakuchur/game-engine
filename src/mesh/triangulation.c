#include "triangulation.h"

#include <stdlib.h>

// Ear clipping method
void triangulate(list_t *face) {
    if (face->size <= 3) return;

    list_t triangulated_face = LIST_INIT;

    while (face->size > 2) {
        int i = 0;
        for (list_node_t *n = face->first; n != NULL && i < 3; n = n->next, i++) {
            vertex_t *m_value = malloc(sizeof(vertex_t));
            *m_value = *(vertex_t *) n->value;
            list_add(&triangulated_face, m_value);
        }
        list_node_t *removed_node = list_remove(face, 1);
        free(removed_node->value);
        free(removed_node);
    }

    list_free(face);
    *face = triangulated_face;
}
