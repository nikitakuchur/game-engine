#ifndef ROGUELIKE_SHADER_H
#define ROGUELIKE_SHADER_H

#include <stdint.h>
#include "math/types.h"

uint32_t shader_create(uint32_t type, const char *source);

uint32_t shader_load(uint32_t type, const char *filename);

uint32_t shader_create_program(uint32_t vertex, uint32_t fragment);

uint32_t shader_create_program_f(const char *vertex_filename, const char *fragment_filename);

void shader_set_int(uint32_t program, const char *name, int value);

void shader_set_float(uint32_t program, const char *name, float value);

void shader_set_vec3(uint32_t program, const char *name, vec3 v);

void shader_set_mat4(uint32_t program, const char *name, mat4 m);

#endif //ROGUELIKE_SHADER_H
