#include "affine.h"

#include <math.h>
#include "mat4.h"
#include "vec4.h"
#include "vec3.h"

void translate(mat4 m, vec3 v) {
    vec4 v1, v2, v3;

    vec4_scale(m[0], v[0], v1);
    vec4_scale(m[1], v[1], v2);
    vec4_scale(m[2], v[2], v3);

    vec4_add(v1, m[3], m[3]);
    vec4_add(v2, m[3], m[3]);
    vec4_add(v3, m[3], m[3]);
}

void scale(mat4 m, vec3 v) {
    vec4_scale(m[0], v[0], m[0]);
    vec4_scale(m[1], v[1], m[1]);
    vec4_scale(m[2], v[2], m[2]);
}

void rotate_x(mat4 m, float angle) {
    mat4 t = MAT4_IDENTITY_INIT;
    float c = cosf(angle);
    float s = sinf(angle);

    t[1][1] =  c;
    t[1][2] =  s;
    t[2][1] = -s;
    t[2][2] =  c;

    mat4_mul(m, t, m);
}

void rotate_y(mat4 m, float angle) {
    mat4 t = MAT4_IDENTITY_INIT;
    float c = cosf(angle);
    float s = sinf(angle);

    t[0][0] =  c;
    t[0][2] = -s;
    t[2][0] =  s;
    t[2][2] =  c;

    mat4_mul(m, t, m);
}

void rotate_z(mat4 m, float angle) {
    mat4 t = MAT4_IDENTITY_INIT;
    float c = cosf(angle);
    float s = sinf(angle);

    t[0][0] =  c;
    t[0][1] =  s;
    t[1][0] = -s;
    t[1][1] =  c;

    mat4_mul(m, t, m);
}

void rotate(mat4 m, float angle, vec3 axis) {
    mat4 rot;
    vec3 axis_norm, vc, vs;
    float c = cosf(angle);

    vec3_normalize_to(axis, axis_norm);
    vec3_scale(axis_norm, 1.0f - c, vc);
    vec3_scale(axis_norm, sinf(angle), vs);

    vec3_scale(axis_norm, vc[0], rot[0]);
    vec3_scale(axis_norm, vc[1], rot[1]);
    vec3_scale(axis_norm, vc[2], rot[2]);

    rot[0][0] += c;     rot[1][0] -= vs[2]; rot[2][0] += vs[1];
    rot[0][1] += vs[2]; rot[1][1] += c;     rot[2][1] -= vs[0];
    rot[0][2] -= vs[1]; rot[1][2] += vs[0]; rot[2][2] += c;

    rot[0][3] = rot[1][3] = rot[2][3] = rot[3][0] = rot[3][1] = rot[3][2] = 0.0f;
    rot[3][3] = 1.0f;

    mat4_mul(m, rot, m);
}
