#ifndef ROGUELIKE_SPECTATOR_SYS_H
#define ROGUELIKE_SPECTATOR_SYS_H

#include <GLFW/glfw3.h>
#include "../components/spectator.h"
#include "../components/transform.h"

void spectator_update(spectator_t *spectator, transform_t *transform, GLFWwindow *window, float dt);

#endif //ROGUELIKE_SPECTATOR_SYS_H
