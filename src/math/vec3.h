#ifndef ROGUELIKE_VEC3_H
#define ROGUELIKE_VEC3_H

#include "types.h"

void vec3_zero(vec3 v);

void vec3_copy(vec3 v1, const vec3 v2);

float vec3_dot(const vec3 a, const vec3 b);

float vec3_norm(vec3 v);

void vec3_cross(const vec3 a, const vec3 b, vec3 dest);

void vec3_add(const vec3 a, const vec3 b, vec3 dest);

void vec3_scale(const vec3 v, float s, vec3 dest);

void vec3_rotate(vec3 v, float angle, vec3 axis);

void vec3_normalize_to(vec3 v, vec3 dest);

#endif //ROGUELIKE_VEC3_H
