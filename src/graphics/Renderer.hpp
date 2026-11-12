//
// Created by Dmitriy on 02.10.2026.
//

#pragma once

#include <memory>

#include "Camera.hpp"
#include "LightSource.hpp"
#include "Mesh.hpp"

namespace assignment::graphics
{
    class Renderer
    {
        std::vector<std::shared_ptr<Mesh>> meshes = {};
        std::vector<LightSource> lights = {};

        [[nodiscard]] glm::vec3 resolveLightPosition(const LightSource& light) const;

    public:
        Camera camera;

        explicit Renderer(const Camera& camera) : camera(camera) {}

        void addMesh(const std::shared_ptr<Mesh>& mesh);
        void addLightSource(const LightSource& lightSource);

        void render() const;
    };
}
