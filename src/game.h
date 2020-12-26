#ifndef ROGUELIKE_GAME_H
#define ROGUELIKE_GAME_H

#include "window.h"

void game_launch(GLFWwindow *window);

static void game_update(float dt);

static void game_draw();

static void game_destroy();

#endif //ROGUELIKE_GAME_H
