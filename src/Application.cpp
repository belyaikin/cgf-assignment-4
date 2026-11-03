//
// Created by Dmitriy on 02.10.2026.
//

#include "Application.hpp"

#include "graphics/window/WindowUtilities.hpp"

namespace assignment
{
    Application::Application(const WindowProperties &windowProperties, const graphics::Camera &camera)
        : renderer(camera)
    {
        if (!glfwInit()) return;

        std::cout << "[Application]: Initialized GLFW\n";

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        window = graphics::window::WindowUtilities::createWindow(
            windowProperties.width,
            windowProperties.height,
            windowProperties.title.c_str()
        );

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        std::cout << "[Application]: "
        << "Renderer: " << glGetString(GL_RENDERER) << ", version: " << glGetString(GL_VERSION) << "\n";

        std::cout << "[Application]: Initialized application\n";
    }

    void Application::addMesh(const graphics::Mesh &mesh) { renderer.addMesh(mesh); }
    void Application::addLightSource(const graphics::LightSource &lightSource) { renderer.addLightSource(lightSource); }

    void Application::run()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            renderer.camera.move(window);
            draw();
        }

        glfwTerminate();
    }

    void Application::draw()
    {
        int xFramebufferSize, yFramebufferSize;
        glfwGetFramebufferSize(window, &xFramebufferSize, &yFramebufferSize);

        if (xFramebufferSize == 0 || yFramebufferSize == 0)
            return;

        renderer.camera.setViewportSize(glm::vec2(xFramebufferSize, yFramebufferSize));
        glViewport(0, 0, xFramebufferSize, yFramebufferSize);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.render();

        glfwSwapBuffers(window);
    }
}
