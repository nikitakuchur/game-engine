#ifndef ROGUELIKE_CAMERA_SYS_H
#define ROGUELIKE_CAMERA_SYS_H

#include "../components/camera.h"
#include "../components/transform.h"

void camera_update(transform_t* transform, GLFWwindow *window, mat4 view, mat4 projection);

#endif //ROGUELIKE_CAMERA_SYS_H
