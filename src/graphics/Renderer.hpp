//
// Created by Dmitriy on 02.10.2026.
//

#pragma once

#include "Camera.hpp"
#include "LightSource.hpp"
#include "Mesh.hpp"

namespace assignment::graphics
{
    class Renderer
    {
        std::vector<Mesh> meshes = {};
        std::vector<LightSource> lights = {};

    public:
        Camera camera;

        explicit Renderer(const Camera& camera) : camera(camera) {}

        void addMesh(const Mesh& mesh);
        void addLightSource(const LightSource& lightSource);

        void render() const;
    };
}
