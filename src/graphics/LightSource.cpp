//
// Created by Dmitriy on 03.11.2026.
//

#include "LightSource.hpp"

namespace assignment::graphics
{
    LightSource::LightSource(const glm::vec3& position, const glm::vec4& color, const bool attachedToCamera)
        : position(position), color(color), attachedToCamera(attachedToCamera) {}
}
