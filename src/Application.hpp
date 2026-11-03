//
// Created by Dmitriy on 02.10.2026.
//

#pragma once
#include <string>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "graphics/Renderer.hpp"

namespace assignment
{
    struct WindowProperties {
        int width;
        int height;
        std::string title;
    };

    class Application
    {
        GLFWwindow *window;

        graphics::Renderer renderer;

        void draw();

    public:
        explicit Application(const WindowProperties &windowProperties, const graphics::Camera &camera);

        [[nodiscard]] graphics::Renderer getRenderer() const { return renderer; }

        void addMesh(const graphics::Mesh &mesh);
        void addLightSource(const graphics::LightSource &lightSource);

        void run();
    };
}
