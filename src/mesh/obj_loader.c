#include "obj_loader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/list.h"
#include "../math/types.h"
#include "triangulation.h"

static void add_vec2_to_list(list_t *list, const vec2 vec) {
    float *v0 = malloc(sizeof(float));
    float *v1 = malloc(sizeof(float));

    *v0 = vec[0];
    *v1 = vec[1];

    list_add(list, v0);
    list_add(list, v1);
}

static void add_vec3_to_list(list_t *list, const vec3 vec) {
    float *v0 = malloc(sizeof(float));
    float *v1 = malloc(sizeof(float));
    float *v2 = malloc(sizeof(float));

    *v0 = vec[0];
    *v1 = vec[1];
    *v2 = vec[2];

    list_add(list, v0);
    list_add(list, v1);
    list_add(list, v2);
}

static void add_v_to_list(list_t *list, char *line) {
    char *p_end;

    float v0 = (float) strtod(line, &p_end);
    float v1 = (float) strtod(p_end, &p_end);
    float v2 = (float) strtod(p_end, NULL);

    add_vec3_to_list(list, (vec3) {v0, v1, v2});
}

static void add_vt_to_list(list_t *list, char *line) {
    char *p_end;

    float v0 = (float) strtod(line, &p_end);
    float v1 = (float) strtod(p_end, NULL);

    add_vec2_to_list(list, (vec2) {v0, v1});
}

static void add_vn_to_list(list_t *list, char *line) {
    add_v_to_list(list, line);
}

static int to_int(const char *str) {
    return (int) strtol(str, NULL, 10);
}

static void process_vertex(list_t *vertex_list, const float *v_array, const float *vn_array, const float *vt_array, char *line) {
    list_t f_line = LIST_INIT;
    char *token = strtok(line, " ");

    // Split the line
    // For example: "42/48/28 40/45/28 34/40/28" -> "42/48/28" "40/45/28" "34/40/28"
    while (token != NULL) {
        char *m_token = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(m_token, token);
        list_add(&f_line, m_token);
        token = strtok(NULL, " ");
    }

    list_t face = LIST_INIT;

    for (list_node_t *n = f_line.first; n != NULL; n = n->next) {
        // Just take the first element as vertex index
        // For example: "42/48/28" -> 42
        vertex_t *vertex = malloc(sizeof(vertex_t));

        char *index_str = strtok(n->value, "/");
        // We need to subtract 1, because indices start at 1 in obj format, but we need to start at 0
        vertex->v_id = to_int(index_str) - 1;
        index_str = strtok(NULL, "/");
        vertex->vt_id = to_int(index_str) - 1;
        index_str = strtok(NULL, "/");
        vertex->vn_id = to_int(index_str) - 1;

        list_add(&face, vertex);
    }

    // Triangulation
    triangulate(&face);

    // Add vertices to vertex list
    for (list_node_t *n = face.first; n != NULL; n = face.first) {
        vertex_t *vertex = n->value;

        float v1 = v_array[vertex->v_id * 3];
        float v2 = v_array[vertex->v_id * 3 + 1];
        float v3 = v_array[vertex->v_id * 3 + 2];

        add_vec3_to_list(vertex_list, (vec3) {v1, v2, v3});

        float vn1 = vn_array[vertex->vn_id * 3];
        float vn2 = vn_array[vertex->vn_id * 3 + 1];
        float vn3 = vn_array[vertex->vn_id * 3 + 2];

        add_vec3_to_list(vertex_list, (vec3) {vn1, vn2, vn3});

        float vt1 = vt_array[vertex->vt_id * 2];
        float vt2 = vt_array[vertex->vt_id * 2 + 1];

        add_vec2_to_list(vertex_list, (vec2) {vt1, vt2});

        list_node_t *first_node = list_remove_first(&face);
        free(first_node->value);
        free(first_node);
    }

    list_free(&f_line);
}

void obj_load(const char *filename, float **vertex_dest, int *vertex_dest_size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot read the file \"%s\"", filename);
        return;
    }
    char *line = NULL;
    size_t len = 0;

    list_t v_list = LIST_INIT;
    list_t vn_list = LIST_INIT;
    list_t vt_list = LIST_INIT;

    // Temporary arrays
    float *v_array = NULL;
    float *vn_array = NULL;
    float *vt_array = NULL;

    list_t vertex_list = LIST_INIT;

    while (getline(&line, &len, file) != -1) {
        if (line[0] == 'v') {
            // Vertex data
            switch (line[1]) {
                case ' ': {
                    // This line is a vertex position
                    // For example: "v -0.203125 0.093750 0.742188"
                    add_v_to_list(&v_list, &line[2]);
                    break;
                }
                case 'n': {
                    // This line is normals
                    // For example: "vn 1.0000 0.0000 0.0000"
                    add_vn_to_list(&vn_list, &line[3]);
                    break;
                }
                case 't': {
                    // This line is texture coords
                    // For example: "vt 0.500000 1.000000"
                    add_vt_to_list(&vt_list, &line[3]);
                    break;
                }
                default:
                    break;
            }
        } else if (line[0] == 'f') {
            // This line is face data
            // For example: "f 42/48/28 40/45/28 34/40/28 32/39/28"
            // It's always at the end of the file.

            // Init temporary arrays when we face the first f line.
            if (v_array == NULL) {
                list_to_float_array(&v_list, &v_array);
            }
            if (vn_array == NULL) {
                list_to_float_array(&vn_list, &vn_array);
            }
            if (vt_array == NULL) {
                list_to_float_array(&vt_list, &vt_array);
            }

            process_vertex(&vertex_list, v_array, vn_array, vt_array, &line[2]);
        }
    }
    fclose(file);
    free(line);

    list_to_float_array(&vertex_list, vertex_dest);
    *vertex_dest_size = vertex_list.size;

    free(v_array);
    free(vn_array);
    free(vt_array);

    // Free lists
    list_free(&v_list);
    list_free(&vt_list);
    list_free(&vn_list);
    list_free(&vertex_list);
}
