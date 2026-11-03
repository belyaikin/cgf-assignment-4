//
// Created by Dmitriy on 03.11.2026.
//

#pragma once
#include "Mesh.hpp"
#include "glm/vec4.hpp"

namespace assignment::graphics
{
    class LightSource final
    {
        glm::vec3 position;
        glm::vec4 color;

    public:
        explicit LightSource(const glm::vec3& position, const glm::vec4& color);

        [[nodiscard]] glm::vec3 getPosition() const { return position; }
        [[nodiscard]] glm::vec4 getColor() const { return color; }
    };
}
