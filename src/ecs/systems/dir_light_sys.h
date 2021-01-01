#ifndef ROGUELIKE_DIR_LIGHT_SYS_H
#define ROGUELIKE_DIR_LIGHT_SYS_H

#include <stdint.h>
#include "../components/dir_light.h"
#include "../../render.h"

void dir_light_draw_shadow_map(dir_light_t *dir_light, uint32_t shader);

void dir_light_draw(dir_light_t *dir_light, uint32_t shader);

#endif //ROGUELIKE_DIR_LIGHT_SYS_H
