#include "mat4.h"

#include "vec4.h"

void mat4_copy(mat4 m, mat4 dest) {
    dest[0][0] = m[0][0]; dest[1][0] = m[1][0];
    dest[0][1] = m[0][1]; dest[1][1] = m[1][1];
    dest[0][2] = m[0][2]; dest[1][2] = m[1][2];
    dest[0][3] = m[0][3]; dest[1][3] = m[1][3];

    dest[2][0] = m[2][0]; dest[3][0] = m[3][0];
    dest[2][1] = m[2][1]; dest[3][1] = m[3][1];
    dest[2][2] = m[2][2]; dest[3][2] = m[3][2];
    dest[2][3] = m[2][3]; dest[3][3] = m[3][3];
}

void mat4_zero(mat4 m) {
    vec4_zero(m[0]);
    vec4_zero(m[1]);
    vec4_zero(m[2]);
    vec4_zero(m[3]);
}

void mat4_identity(mat4 m) {
    mat4 im = MAT4_IDENTITY_INIT;
    mat4_copy(im, m);
}

void mat4_mul(mat4 m1, mat4 m2, mat4 dest) {
    float a00 = m1[0][0], a01 = m1[0][1], a02 = m1[0][2], a03 = m1[0][3],
            a10 = m1[1][0], a11 = m1[1][1], a12 = m1[1][2], a13 = m1[1][3],
            a20 = m1[2][0], a21 = m1[2][1], a22 = m1[2][2], a23 = m1[2][3],
            a30 = m1[3][0], a31 = m1[3][1], a32 = m1[3][2], a33 = m1[3][3],

            b00 = m2[0][0], b01 = m2[0][1], b02 = m2[0][2], b03 = m2[0][3],
            b10 = m2[1][0], b11 = m2[1][1], b12 = m2[1][2], b13 = m2[1][3],
            b20 = m2[2][0], b21 = m2[2][1], b22 = m2[2][2], b23 = m2[2][3],
            b30 = m2[3][0], b31 = m2[3][1], b32 = m2[3][2], b33 = m2[3][3];

    dest[0][0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
    dest[0][1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
    dest[0][2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
    dest[0][3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
    dest[1][0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
    dest[1][1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
    dest[1][2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
    dest[1][3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
    dest[2][0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
    dest[2][1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
    dest[2][2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
    dest[2][3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
    dest[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
    dest[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
    dest[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
    dest[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
}
