#ifndef ROGUELIKE_CAM_H
#define ROGUELIKE_CAM_H

#include "types.h"

void ortho(float left, float right, float bottom, float top, float nearZ, float farZ, mat4 dest);

void perspective(float fov_y, float aspect, float near_z, float far_z, mat4 dest);

#endif //ROGUELIKE_CAM_H
