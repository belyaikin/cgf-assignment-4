//
// Created by Dmitriy on 18.10.26.
//

#include "WindowUtilities.hpp"

namespace assignment::graphics::window
{
    void WindowUtilities::windowSizeChangeCallback(GLFWwindow *window, const int width, const int height)
    {
        glViewport(0, 0, width, height);
    }

    GLFWwindow * WindowUtilities::createWindow(const int width, const int height, const char* title)
    {
        GLFWwindow* window = glfwCreateWindow(
            width, height,
            title,
            nullptr, nullptr
        );

        if (!window) {
            std::cout << "[WindowUtilities]: Failed to create GLFW window\n";
            glfwTerminate();
            return nullptr;
        }

        glfwMakeContextCurrent(window);
        glfwSetWindowSizeCallback(window, windowSizeChangeCallback);

        if (!gladLoadGL(glfwGetProcAddress)) {
            std::cout << "[WindowUtilities]: Failed to initialize GLAD\n";
            glfwTerminate();
            return nullptr;
        }

        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        std::cout << "[WindowUtilities]: Created window\n";
        return window;
    }
}
