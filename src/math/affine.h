#ifndef ROGUELIKE_AFFINE_H
#define ROGUELIKE_AFFINE_H

#include "types.h"

void translate(mat4 m, vec3 v);

void scale(mat4 m, vec3 v);

void rotate_x(mat4 m, float angle);

void rotate_y(mat4 m, float angle);

void rotate_z(mat4 m, float angle);

void rotate(mat4 m, float angle, vec3 axis);

#endif //ROGUELIKE_AFFINE_H
