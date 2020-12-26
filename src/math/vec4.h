#ifndef ROGUELIKE_VEC4_H
#define ROGUELIKE_VEC4_H

#include "types.h"

void vec4_zero(vec4 v);

void vec4_add(const vec4 a, const vec4 b, vec4 dest);

void vec4_scale(const vec4 v, float s, vec4 dest);

#endif //ROGUELIKE_VEC4_H
