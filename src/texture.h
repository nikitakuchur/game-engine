#ifndef ROGUELIKE_TEXTURE_H
#define ROGUELIKE_TEXTURE_H

#include <stdint.h>

uint32_t texture_create_empty();

uint32_t texture_load_rgba(const char* filename);

uint32_t texture_load(const char* filename, uint32_t format);

#endif //ROGUELIKE_TEXTURE_H
