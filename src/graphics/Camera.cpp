//
// Created by Dmitriy on 22.10.26.
//

#define GLFW_INCLUDE_NONE
#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.hpp"

#include "GLFW/glfw3.h"
#include <glm/gtx/rotate_vector.hpp>
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/gtx/vector_angle.hpp"

namespace assignment::graphics
{
    Camera::CameraMatrix Camera::getMatrix() const {
        auto matrix = CameraMatrix{.view = glm::mat4x4(1.0f), .projection = glm::mat4x4(1.0f)};

        matrix.view = glm::lookAt(position, position + orientation, up);
        matrix.projection = glm::perspective(glm::radians(fov), viewportSize.x / viewportSize.y, .1f, 100.0f);

        return matrix;
    }

    void Camera::move(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            position += speed * orientation;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            position -= speed * orientation;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position += speed * -glm::normalize(glm::cross(orientation, up));

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position += speed * glm::normalize(glm::cross(orientation, up));

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            position += speed * -up;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            position += speed * up;

        float rotX = 0.f;
        float rotY = 0.f;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rotY -= 1.f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rotY += 1.f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) rotX -= 1.f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) rotX += 1.f;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            if (firstClick)
            {
                glfwSetCursorPos(window, viewportSize.x / 2, viewportSize.y / 2);
                firstClick = false;
            }

            double mouseX, mouseY;

            glfwGetCursorPos(window, &mouseX, &mouseY);

            rotX += sensitivity * static_cast<float>(mouseY - viewportSize.y / 2) / viewportSize.y;
            rotY += sensitivity * static_cast<float>(mouseX - viewportSize.x / 2) / viewportSize.x;

            glfwSetCursorPos(window, viewportSize.x / 2, viewportSize.y / 2);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstClick = true;
        }

        const glm::vec3 newOrientation = glm::rotate(
            orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

        if (glm::angle(newOrientation, up) > glm::radians(5.0f) &&
            glm::angle(newOrientation, -up) > glm::radians(5.0f)) {
            orientation = newOrientation;
        }

        orientation = glm::rotate(orientation, glm::radians(-rotY), up);
    }
}
