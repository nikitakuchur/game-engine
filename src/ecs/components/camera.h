#ifndef ROGUELIKE_CAMERA_H
#define ROGUELIKE_CAMERA_H

#include <GLFW/glfw3.h>
#include "../../math/types.h"

typedef struct {
    size_t entity_id;
    int active;
} camera_t;

#endif //ROGUELIKE_CAMERA_H
