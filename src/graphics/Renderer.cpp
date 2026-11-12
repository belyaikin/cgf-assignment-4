//
// Created by Dmitriy on 02.10.2026.
//

#include "Renderer.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace assignment::graphics
{
    void Renderer::addMesh(const std::shared_ptr<Mesh> &mesh) {
        this->meshes.push_back(mesh);
    }

    void Renderer::addLightSource(const LightSource &lightSource)
    {
        this->lights.push_back(lightSource);
    }

    glm::vec3 Renderer::resolveLightPosition(const LightSource &light) const
    {
        if (!light.isAttachedToCamera())
            return light.getPosition();

        const glm::vec3 forward = glm::normalize(camera.getOrientation());
        const glm::vec3 right = glm::normalize(glm::cross(forward, camera.getUp()));
        const glm::vec3 up = glm::cross(right, forward);
        const glm::vec3 offset = light.getPosition();

        return camera.getPosition() + offset.x * right + offset.y * up + offset.z * forward;
    }

    void Renderer::render() const
    {
        auto [view, projection] = camera.getMatrix();

        std::vector<glm::vec3> lightPositions;
        std::vector<glm::vec4> lightColors;
        lightPositions.reserve(this->lights.size());
        lightColors.reserve(this->lights.size());

        for (const auto& light : this->lights) {
            lightPositions.push_back(resolveLightPosition(light));
            lightColors.push_back(light.getColor());
        }

        const int numActiveLights = static_cast<int>(this->lights.size());

        for (const auto& mesh : this->meshes)
        {
            const GLuint shaderId = mesh->getShaderProgram().getId();

            glUseProgram(shaderId);

            const glm::mat4 model = glm::translate(
                glm::mat4(1.0f),
                mesh->getPosition()
            );

            glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shaderId, "proj"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniform3fv(glGetUniformLocation(shaderId, "viewPosition"), 1, glm::value_ptr(camera.getPosition()));

            glUniform1i(glGetUniformLocation(shaderId, "numLights"), numActiveLights);

            if (numActiveLights > 0) {
                glUniform3fv(
                    glGetUniformLocation(shaderId, "lightPositions[0]"),
                    numActiveLights,
                    glm::value_ptr(lightPositions[0])
                );

                glUniform4fv(
                    glGetUniformLocation(shaderId, "lightColors[0]"),
                    numActiveLights,
                    glm::value_ptr(lightColors[0])
                );
            }

            mesh->draw({.model = model, .view = view, .projection = projection});
        }
    }
}
