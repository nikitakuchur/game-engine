#include "cam.h"

#include <math.h>
#include "mat4.h"

void ortho(float left, float right, float bottom, float top, float nearZ, float farZ, mat4 dest) {
    mat4_zero(dest);

    float rl = 1.0f / (right - left);
    float tb = 1.0f / (top - bottom);
    float fn = -1.0f / (farZ - nearZ);

    dest[0][0] = 2.0f * rl;
    dest[1][1] = 2.0f * tb;
    dest[2][2] = 2.0f * fn;
    dest[3][0] = -(right + left) * rl;
    dest[3][1] = -(top + bottom) * tb;
    dest[3][2] = (farZ + nearZ) * fn;
    dest[3][3] = 1.0f;
}

void perspective(float fov_y, float aspect, float near_z, float far_z, mat4 dest) {
    mat4_zero(dest);

    float f = 1.0f / tanf(fov_y * 0.5f);
    float fn = 1.0f / (near_z - far_z);

    dest[0][0] = f / aspect;
    dest[1][1] = f;
    dest[2][2] = (near_z + far_z) * fn;
    dest[2][3] = -1.0f;
    dest[3][2] = 2.0f * near_z * far_z * fn;
}
