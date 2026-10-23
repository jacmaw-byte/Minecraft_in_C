//
// Created by jacobmawby on 20/09/2026.
//

#include <glad.h>
#include "glfw_stuff.h"
#include "cglm/cglm.h"
#include "globals.h"

GLFWwindow* init_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Minecraft", nullptr, nullptr);

    if (window == nullptr) {
        printf("Failed to create window \n");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("failed to load GLAD \n");
        exit(-1);
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);

    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (hit.hit) {
            destroy_block(&hit);
        }

    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (hit.hit) {
            place_block(&hit);
        }
    }
}

void processInput(GLFWwindow *window)
{
    printf("%s \n", glfwGetVersionString());

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = delta_time * player.cam.speed; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 scalar;
        glm_vec3_scale(player.cam.direction, cameraSpeed, scalar);
        glm_vec3_add(scalar, player.cam.pos, player.cam.pos);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 scalar;
        glm_vec3_scale(player.cam.direction, cameraSpeed, scalar);
        glm_vec3_sub(player.cam.pos, scalar, player.cam.pos);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 front;
        glm_cross(player.cam.direction, player.cam.up, front);
        glm_normalize(front);
        vec3 scalar;
        glm_vec3_scale(front, cameraSpeed, scalar);
        glm_vec3_sub(player.cam.pos, scalar, player.cam.pos);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 front;
        glm_cross(player.cam.direction, player.cam.up, front);
        glm_normalize(front);
        vec3 scalar;
        glm_vec3_scale(front, cameraSpeed, scalar);
        glm_vec3_add(player.cam.pos, scalar, player.cam.pos);
    }
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;
    last_x = xpos;
    last_y = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    player.cam.yaw   += xoffset;
    player.cam.pitch += yoffset;

    if(player.cam.pitch > 89.0f)
        player.cam.pitch = 89.0f;
    if(player.cam.pitch < -89.0f)
        player.cam.pitch = -89.0f;

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        player.cam.speed += (float)yoffset;
        if (player.cam.speed < 5.0f) {
            player.cam.speed = 5.0f;
        }
    }

}