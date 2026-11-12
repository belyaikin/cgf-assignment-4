//
// Created by Dmitriy on 11.11.2026.
//

#pragma once

#include <array>
#include <vector>

#include "Mesh.hpp"
#include "glm/vec3.hpp"
#include "opengl/object/Texture.hpp"

namespace assignment::graphics
{
    class BezierPatch final : public Mesh
    {
    public:
        static constexpr int ORDER = 4;
        static constexpr int CONTROL_POINT_COUNT = ORDER * ORDER;
        static constexpr int MIN_RESOLUTION = 2;
        static constexpr int MAX_RESOLUTION = 100;

        using ControlPoints = std::array<glm::vec3, CONTROL_POINT_COUNT>;

        enum class ShadingMode { Flat, Textured };

        struct Shaders {
            opengl::object::shader::ShaderProgram flat;
            opengl::object::shader::ShaderProgram textured;
            opengl::object::shader::ShaderProgram overlay;
        };

        BezierPatch(
            const ControlPoints &controlPoints,
            const Shaders &shaders,
            opengl::object::Texture texture,
            glm::vec3 position,
            int resolution = 10,
            ShadingMode shadingMode = ShadingMode::Flat
        );

        static ControlPoints loadControlPoints(const char *filename);

        void draw(const RenderContext &context) const override;

        void selectNextControlPoint();
        void selectPreviousControlPoint();
        void moveSelectedControlPoint(const glm::vec3 &delta);

        void setResolution(int value);
        void increaseResolution() { setResolution(resolution + 1); }
        void decreaseResolution() { setResolution(resolution - 1); }

        void setShadingMode(ShadingMode mode);
        void toggleShadingMode();

        [[nodiscard]] int getResolution() const { return resolution; }
        [[nodiscard]] int getSelectedIndex() const { return selectedIndex; }
        [[nodiscard]] ShadingMode getShadingMode() const { return shadingMode; }
        [[nodiscard]] const ControlPoints &getControlPoints() const { return controlPoints; }

        [[nodiscard]] static glm::vec3 computeCenter(const ControlPoints &points);

    private:
        struct SampleGrid {
            int size;
            std::vector<glm::vec3> positions;
            std::vector<glm::vec3> normals;
        };

        struct Tessellation {
            VertexData vertexData;
            std::vector<GLfloat> centroids;
        };

        ControlPoints controlPoints;
        int resolution;
        int selectedIndex = 0;
        ShadingMode shadingMode;

        glm::vec3 diffuseColor{0.85f, 0.55f, 0.2f};

        float ambientStrength = 0.15f;
        glm::vec3 specularColor{0.6f, 0.6f, 0.6f};
        float shininess = 32.f;

        float axisLength = 1.f;
        float pointSize = 8.f;
        float selectedPointSize = 16.f;

        Shaders shaders;
        opengl::object::Texture texture;

        opengl::object::VertexBuffer centroidBuffer;

        opengl::object::VertexArray controlPointsArray;
        opengl::object::VertexBuffer controlPointsBuffer;

        opengl::object::VertexArray axesArray;
        opengl::object::VertexBuffer axesBuffer;

        static SampleGrid evaluate(const ControlPoints &points, int resolution);
        static Tessellation tessellateFlat(const SampleGrid &grid);
        static Tessellation tessellateSmooth(const SampleGrid &grid);

        void retessellate();
        void uploadCentroids(const std::vector<GLfloat> &centroids) const;
        void uploadControlPoints() const;
        void uploadAxes() const;

        void setSurfaceUniforms() const;

        static void setupOverlayVertexArray(GLuint vertexArrayId, GLuint bufferId);
        void setOverlayUniforms(const RenderContext &context) const;
    };
}
