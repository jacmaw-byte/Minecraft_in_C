//
// Created by jacobmawby on 20/09/2026.
//

#ifndef MINECRAFT_IN_C_GLFW_STUFF_H
#define MINECRAFT_IN_C_GLFW_STUFF_H
#include <GLFW/glfw3.h>

GLFWwindow* init_glfw();
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif //MINECRAFT_IN_C_GLFW_STUFF_H
