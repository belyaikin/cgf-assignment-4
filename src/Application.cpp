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

        // The window's own framebuffer is single-sampled; anti-aliasing is done
        // through an off-screen multisampled FBO so it can be switched at runtime.
        glfwWindowHint(GLFW_SAMPLES, 0);

        window = graphics::window::WindowUtilities::createWindow(
            windowProperties.width,
            windowProperties.height,
            windowProperties.title.c_str()
        );

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        multisampleFramebuffer.emplace(4);

        std::cout << "[Application]: "
        << "Renderer: " << glGetString(GL_RENDERER) << ", version: " << glGetString(GL_VERSION) << "\n";

        std::cout << "[Application]: Initialized application\n";
        std::cout << "[Application]: Anti-aliasing " << (antiAliasingEnabled ? "ON" : "OFF")
                  << " (" << multisampleFramebuffer->getSamples() << "x MSAA available)\n";
    }

    void Application::setAntiAliasingEnabled(const bool enabled)
    {
        antiAliasingEnabled = enabled;
        std::cout << "[Application]: Anti-aliasing " << (enabled ? "ON" : "OFF") << "\n";
    }

    void Application::addMesh(const std::shared_ptr<graphics::Mesh> &mesh) { renderer.addMesh(mesh); }

    void Application::setKeyCallback(std::function<void(int key, int action, int mods)> callback)
    {
        keyCallback = std::move(callback);
    }

    void Application::onKey(GLFWwindow *window, const int key, int, const int action, const int mods)
    {
        const auto *application = static_cast<Application *>(glfwGetWindowUserPointer(window));
        if (application && application->keyCallback)
            application->keyCallback(key, action, mods);
    }
    void Application::addLightSource(const graphics::LightSource &lightSource) { renderer.addLightSource(lightSource); }

    void Application::run()
    {
        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, onKey);

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

        if (antiAliasingEnabled)
        {
            multisampleFramebuffer->resize(xFramebufferSize, yFramebufferSize);
            multisampleFramebuffer->bind();
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        glViewport(0, 0, xFramebufferSize, yFramebufferSize);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.render();

        if (antiAliasingEnabled)
            multisampleFramebuffer->resolveToDefaultFramebuffer();

        glfwSwapBuffers(window);
    }
}
