#include <iostream>
#include <string>
#include <stdlib.h>
#include "window_manager.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
#include "buffers.h"
#include "framebuffers.h"

#include <glm/gtc/matrix_transform.hpp>

struct mouse : public MouseListener
{
    /* data */
    float x;
    float y;
    void process_mouse(float new_x, float new_y)
    {
        x = new_x;
        y = 800 - new_y;
    }
};

int main()
{
    WindowManager wm;
    wm.init(800, 800, "Hello Foveation");
    gladLoadGL();

    InputManager im;
    Camera cam;
    im.add_keyboard_listener(cam);
    // im.add_mouse_listener(cam);
    // im.add_scroll_listener(cam);
    wm.set_input_manager(im);
    mouse mousePosition;
    im.add_mouse_listener(mousePosition);

    Shader shader("./shaders/model.vs", "./shaders/model.fs");
    Shader fbShader("./shaders/framebuffer.vs", "./shaders/framebuffer.fs");

    Model sphere("/home/kaymk11/Code/Projects/Foveation/resources/models/sphere.obj");
    Model cube("/home/kaymk11/Code/Projects/Foveation/resources/models/containerbox.obj");
    Model plane("/home/kaymk11/Code/Projects/Foveation/resources/models/grassyGround2.obj");

    float quadVertices[] = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                            // positions   // texCoords
                            -1.0f, 1.0f, 0.0f, 1.0f,
                            -1.0f, -1.0f, 0.0f, 0.0f,
                            1.0f, -1.0f, 1.0f, 0.0f,

                            -1.0f, 1.0f, 0.0f, 1.0f,
                            1.0f, -1.0f, 1.0f, 0.0f,
                            1.0f, 1.0f, 1.0f, 1.0f};
    float foveaquad[] = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                         // positions   // texCoords
                         -0.125f, 0.125f, 0.0f, 1.0f,
                         -0.125f, -0.125f, 0.0f, 0.0f,
                         0.125f, -0.125f, 1.0f, 0.0f,

                         -0.125f, 0.125f, 0.0f, 1.0f,
                         0.125f, -0.125f, 1.0f, 0.0f,
                         0.125f, 0.125f, 1.0f, 1.0f};

    VertexArray quadVAO;
    quadVAO.bind();
    VertexBuffer quadVBO(quadVertices, sizeof(quadVertices));
    quadVBO.setLayout(0, 2, 4, 0);
    quadVBO.setLayout(1, 2, 4, 2);
    quadVBO.bind();
    quadVAO.unbind();
    quadVBO.unbind();

    VertexArray foveaVAO;
    foveaVAO.bind();
    VertexBuffer foveaVBO(foveaquad, sizeof(foveaquad));
    foveaVBO.setLayout(0, 2, 4, 0);
    foveaVBO.setLayout(1, 2, 4, 2);
    foveaVBO.bind();
    foveaVAO.unbind();
    foveaVBO.unbind();

    fbShader.use();
    fbShader.setInt("screenTexture", 0);

    FrameBuffer fbo1(800, 800, true);
    FrameBuffer fbo2(600, 600, true);
    FrameBuffer fbo3(400, 400, true);
    FrameBuffer fbo4(200, 200, true);

    FrameBuffer fovea(100, 100, true);

    FrameBuffer *fbo = &fbo1;
    float fov = 5.9f;
    wm.bindDefaultFrameBuffer();
    while (wm.isWindowActive())
    {
        if (glfwGetKey(wm.window, GLFW_KEY_1) == GLFW_PRESS)
            fbo = &fbo1;
        if (glfwGetKey(wm.window, GLFW_KEY_2) == GLFW_PRESS)
            fbo = &fbo2;
        if (glfwGetKey(wm.window, GLFW_KEY_3) == GLFW_PRESS)
            fbo = &fbo3;
        if (glfwGetKey(wm.window, GLFW_KEY_4) == GLFW_PRESS)
            fbo = &fbo4;
        if (glfwGetKey(wm.window, GLFW_KEY_UP) == GLFW_PRESS)
            fov += 0.05f;
        if (glfwGetKey(wm.window, GLFW_KEY_DOWN) == GLFW_PRESS)
            fov -= 0.05f;

        // if (glfwGetKey(wm.window, GLFW_KEY_5) == GLFW_PRESS)
        //     fbo = &fovea;
        fbo->bind();
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        shader.use();
        shader.setMat4("projection", cam.getProjectionMatrix());
        shader.setMat4("view", cam.getViewMatrix());
        shader.setMat4("model", glm::mat4(1.0f));

        glm::mat4 plane_model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.0f, 0.f));
        plane_model = glm::scale(plane_model, glm::vec3(50.f, 50.f, 50.f));
        shader.setMat4("model", plane_model);
        plane.draw(shader);

        glm::mat4 cube_model = glm::translate(glm::mat4(1.f), glm::vec3(-1.5, 0.f, -10.f));
        shader.setMat4("model", cube_model);
        cube.draw(shader);

        glm::mat4 sphere_model = glm::translate(glm::mat4(1.f), glm::vec3(.5, 0.f, -20.f));
        shader.setMat4("model", sphere_model);
        sphere.draw(shader);

        fovea.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::vec3 end = glm::unProject(glm::vec3(mousePosition.x, mousePosition.y, 0.f), cam.getViewMatrix(), cam.getProjectionMatrix(), glm::vec4(0, 0, 800, 800));
        glm::vec3 forward = end - cam.position;
        glm::vec2 foveaoffset((mousePosition.x - 400.0f) / 400.0f, (mousePosition.y - 400.f) / 400.f);
        forward = glm::normalize(forward);

        shader.setMat4("projection", glm::perspective(glm::radians(fov), (float)800 / (float)800, 0.1f, 100.0f));
        std::cout << fov << std::endl;
        shader.setMat4("view", glm::lookAt(cam.position, cam.position + forward, cam.up));
        shader.setMat4("model", glm::mat4(1.0f));

        plane_model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.0f, 0.f));
        plane_model = glm::scale(plane_model, glm::vec3(50.f, 50.f, 50.f));
        shader.setMat4("model", plane_model);
        plane.draw(shader);

        cube_model = glm::translate(glm::mat4(1.f), glm::vec3(-1.5, 0.f, -10.f));
        shader.setMat4("model", cube_model);
        cube.draw(shader);

        sphere_model = glm::translate(glm::mat4(1.f), glm::vec3(.5, 0.f, -20.f));
        shader.setMat4("model", sphere_model);
        sphere.draw(shader);
        wm.bindDefaultFrameBuffer();
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        fbShader.use();
        fbShader.setVec2("offset", 0.f, 0.f);
        quadVAO.bind();
        glBindTexture(GL_TEXTURE_2D, fbo->getTextureBuffer());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        fbShader.setVec2("offset", foveaoffset);
        foveaVAO.bind();
        glBindTexture(GL_TEXTURE_2D, fovea.getTextureBuffer());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        wm.updateWindow();
    }
    wm.close();
    return 0;
}
