//
// Created by Dmitriy on 22.10.26.
//

#pragma once

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

struct GLFWwindow;

namespace assignment::graphics {
    class Camera final {
        bool firstClick = true;

        struct CameraMatrix {
            glm::mat4x4 view;
            glm::mat4x4 projection;
        };

        glm::vec3 position;
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec2 viewportSize{};

        float speed = 0.1f;
        float sensitivity = 100.f;

        float fov = 45.0f;
    public:
        explicit Camera(const glm::vec3& position) : position(position) {}

        [[nodiscard]] const glm::vec2& getViewportSize() const { return viewportSize; }
        void setViewportSize(const glm::vec2& value) { viewportSize = value; }

        [[nodiscard]] CameraMatrix getMatrix() const;

        void move(GLFWwindow* window);
    };
}
