#ifndef ROGUELIKE_POINT_LIGHT_SYS_H
#define ROGUELIKE_POINT_LIGHT_SYS_H

#include <stdint.h>
#include "../components/point_light.h"

void point_light_draw(point_light_t *point_light, uint32_t shader);

#endif //ROGUELIKE_POINT_LIGHT_SYS_H
