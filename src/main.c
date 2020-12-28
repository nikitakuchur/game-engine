#include "window.h"
#include "game.h"

int main() {
    glfwInit();
    GLFWwindow *window = window_create(1280, 720, "roguelike");
    glfwSetCursorPos(window, 0.f, 0.f);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    game_launch(window);

    glfwTerminate();
    return 0;
}
