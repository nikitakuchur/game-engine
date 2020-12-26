#ifndef ROGUELIKE_WINDOW_H
#define ROGUELIKE_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow *window_create(int width, int height, const char *title);

#endif //ROGUELIKE_WINDOW_H
