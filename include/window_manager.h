#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "input_manager.h"

InputManager *im;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (im == NULL)
        return;
    InputEvent event;
    event.type = MOUSE;
    event.x = (float)xpos;
    event.y = (float)ypos;
    im->process(event);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (im == NULL)
        return;
    InputEvent event;
    event.y = (float)yoffset;
    im->process(event);
}
class WindowManager
{
public:
    GLFWwindow *window;
    int height, width;
    float deltaTime = 0.f;
    float lastFrame = 0.f;
    std::string title;
    void processInput(float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (im == NULL)
            return;

        InputEvent event;
        event.type = KEYBOARD;
        event.delta_time = deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            event.pressed_keys.push_back(GLFW_KEY_W);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            event.pressed_keys.push_back(GLFW_KEY_A);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            event.pressed_keys.push_back(GLFW_KEY_S);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            event.pressed_keys.push_back(GLFW_KEY_D);

        im->process(event);
    }

    int init(int height, int width, std::string title)
    {
        this->height = height;
        this->width = width;
        this->title = title;

        if (!glfwInit())
            return false;

        window = glfwCreateWindow(height, width, title.c_str(), NULL, NULL);

        if (!window)
        {
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glfwSetCursorPosCallback(window, mouse_callback);
        // glfwSetScrollCallback(window, scroll_callback);
        return true;
    }

    void set_input_manager(InputManager &_im)
    {
        im = &_im;
    }
    int isWindowActive()
    {
        return !glfwWindowShouldClose(window);
    }
    float updateWindow()
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwSwapBuffers(window);
        glfwPollEvents();
        processInput(deltaTime);

        return deltaTime;
    }
    void close()
    {
        glfwTerminate();
    }
    void bindDefaultFrameBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
    }
};

#endif
