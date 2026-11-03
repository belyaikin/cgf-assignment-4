//
// Created by Dmitriy on 02.10.2026.
//

#include "Renderer.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace assignment::graphics
{
    void Renderer::addMesh(const Mesh &mesh) {
        this->meshes.push_back(mesh);
    }

    void Renderer::addLightSource(const LightSource &lightSource)
    {
        this->lights.push_back(lightSource);
    }

    void Renderer::render() const
    {
        auto [view, projection] = camera.getMatrix();

        for (const Mesh& mesh : this->meshes)
        {
            const GLuint shaderId = mesh.getShaderProgram().getId();

            glUseProgram(shaderId);
            glBindVertexArray(mesh.getVertexArray().getId());

            const glm::mat4 model = glm::translate(
                glm::mat4(1.0f),
                mesh.getPosition()
            );

            const GLint modelLoc = glGetUniformLocation(shaderId, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            const GLint viewLoc = glGetUniformLocation(shaderId, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            const GLint projLoc = glGetUniformLocation(shaderId, "proj");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            const int numActiveLights = static_cast<int>(this->lights.size());
            glUniform1i(glGetUniformLocation(shaderId, "numLights"), numActiveLights);

            std::vector<glm::vec3> lightPositions;
            std::vector<glm::vec4> lightColors;
            lightPositions.reserve(this->lights.size());
            lightColors.reserve(this->lights.size());

            for (const auto& light : this->lights) {
                lightPositions.push_back(light.getPosition());
                lightColors.push_back(light.getColor());
            }

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

            if (!mesh.getVertexData().elements.empty()) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getElementBuffer().getId());
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.getVertexData().elements.size()), GL_UNSIGNED_INT, nullptr);
            } else {
                glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.getVertexData().vertices.size() / 3));
            }

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }
}
