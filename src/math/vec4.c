#include "vec4.h"

void vec4_zero(vec4 v) {
    v[0] = v[1] = v[2] = v[3] = 0.0f;
}

void vec4_add(const vec4 a, const vec4 b, vec4 dest) {
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
    dest[2] = a[2] + b[2];
    dest[3] = a[3] + b[3];
}

void vec4_scale(const vec4 v, float s, vec4 dest) {
    dest[0] = v[0] * s;
    dest[1] = v[1] * s;
    dest[2] = v[2] * s;
    dest[3] = v[3] * s;
}
