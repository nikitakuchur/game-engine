#include "vec3.h"

#include <math.h>

void vec3_zero(vec3 v) {
    v[0] = v[1] = v[2] = 0.0f;
}

void vec3_copy(vec3 v1, const vec3 v2) {
    v1[0] = v2[0];
    v1[1] = v2[1];
    v1[2] = v2[2];
}

float vec3_dot(const vec3 a, const vec3 b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

float vec3_norm(vec3 v) {
    return sqrtf(vec3_dot(v, v));
}

void vec3_cross(const vec3 a, const vec3 b, vec3 dest) {
    dest[0] = a[1] * b[2] - a[2] * b[1];
    dest[1] = a[2] * b[0] - a[0] * b[2];
    dest[2] = a[0] * b[1] - a[1] * b[0];
}

void vec3_add(const vec3 a, const vec3 b, vec3 dest) {
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
    dest[2] = a[2] + b[2];
}

void vec3_scale(const vec3 v, float s, vec3 dest) {
    dest[0] = v[0] * s;
    dest[1] = v[1] * s;
    dest[2] = v[2] * s;
}

void vec3_rotate(vec3 v, float angle, vec3 axis) {
    vec3 v1, v2, k;

    float c = cosf(angle);
    float s = sinf(angle);

    vec3_normalize_to(axis, k);

    vec3_scale(v, c, v1);

    vec3_cross(k, v, v2);
    vec3_scale(v2, s, v2);

    vec3_add(v1, v2, v1);

    vec3_scale(k, vec3_dot(k, v) * (1.0f - c), v2);
    vec3_add(v1, v2, v);
}

void vec3_normalize_to(vec3 v, vec3 dest) {
    float norm = vec3_norm(v);

    if (norm == 0.0f) {
        vec3_zero(dest);
        return;
    }
    vec3_scale(v, 1.0f / norm, dest);
}
