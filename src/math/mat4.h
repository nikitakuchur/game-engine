#ifndef ROGUELIKE_MAT4_H
#define ROGUELIKE_MAT4_H

#include "types.h"

#define MAT4_IDENTITY_INIT  {{1.0f, 0.0f, 0.0f, 0.0f},\
                             {0.0f, 1.0f, 0.0f, 0.0f},\
                             {0.0f, 0.0f, 1.0f, 0.0f},\
                             {0.0f, 0.0f, 0.0f, 1.0f}}

void mat4_copy(mat4 m, mat4 dest);

void mat4_zero(mat4 m);

void mat4_identity(mat4 m);

void mat4_mul(mat4 m1, mat4 m2, mat4 dest);

#endif //ROGUELIKE_MAT4_H
