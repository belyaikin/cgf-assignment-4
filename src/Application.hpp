//
// Created by Dmitriy on 02.10.2026.
//

#pragma once
#include <functional>
#include <memory>
#include <optional>
#include <string>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "graphics/MultisampleFramebuffer.hpp"
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

        // Created after the OpenGL context exists.
        std::optional<graphics::MultisampleFramebuffer> multisampleFramebuffer;
        bool antiAliasingEnabled = true;

        std::function<void(int key, int action, int mods)> keyCallback;

        static void onKey(GLFWwindow *window, int key, int scancode, int action, int mods);

        void draw();

    public:
        explicit Application(const WindowProperties &windowProperties, const graphics::Camera &camera);

        [[nodiscard]] graphics::Renderer getRenderer() const { return renderer; }

        void addMesh(const std::shared_ptr<graphics::Mesh> &mesh);

        void setKeyCallback(std::function<void(int key, int action, int mods)> callback);
        void addLightSource(const graphics::LightSource &lightSource);

        [[nodiscard]] bool isAntiAliasingEnabled() const { return antiAliasingEnabled; }
        void setAntiAliasingEnabled(bool enabled);
        void toggleAntiAliasing() { setAntiAliasingEnabled(!antiAliasingEnabled); }

        void run();
    };
}
