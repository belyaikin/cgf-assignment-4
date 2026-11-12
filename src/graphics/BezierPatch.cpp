//
// Created by Dmitriy on 11.11.2026.
//

#include "BezierPatch.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../files/FileUtilities.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace assignment::graphics
{
    namespace
    {
        std::array<float, BezierPatch::ORDER> bernstein(const float t)
        {
            const float s = 1.f - t;
            return {s * s * s, 3.f * t * s * s, 3.f * t * t * s, t * t * t};
        }

        void appendVec3(std::vector<GLfloat> &target, const glm::vec3 &v)
        {
            target.insert(target.end(), {v.x, v.y, v.z});
        }

        void appendOverlayVertex(std::vector<GLfloat> &target, const glm::vec3 &position, const glm::vec3 &color)
        {
            target.insert(target.end(), {position.x, position.y, position.z, color.x, color.y, color.z});
        }

        std::array<float, BezierPatch::ORDER> bernsteinDerivative(const float t)
        {
            const float s = 1.f - t;
            return {-3.f * s * s, 3.f * s * s - 6.f * t * s, 6.f * t * s - 3.f * t * t, 3.f * t * t};
        }

        void appendVec2(std::vector<GLfloat> &target, const float x, const float y)
        {
            target.insert(target.end(), {x, y});
        }

        constexpr GLuint CENTROID_ATTRIBUTE = 3;

        constexpr glm::vec3 CONTROL_POINT_COLOR{1.f, 1.f, 1.f};
        constexpr glm::vec3 SELECTED_CONTROL_POINT_COLOR{1.f, 0.9f, 0.f};
    }

    BezierPatch::BezierPatch(
        const ControlPoints &controlPoints,
        const Shaders &shaders,
        const opengl::object::Texture texture,
        const glm::vec3 position,
        const int resolution,
        const ShadingMode shadingMode
    )
        : Mesh(VertexData{}, shadingMode == ShadingMode::Flat ? shaders.flat : shaders.textured, position),
          controlPoints(controlPoints),
          resolution(std::clamp(resolution, MIN_RESOLUTION, MAX_RESOLUTION)),
          shadingMode(shadingMode),
          shaders(shaders),
          texture(texture),
          centroidBuffer(generateBuffer()),
          controlPointsArray(generateVertexArray()),
          controlPointsBuffer(generateBuffer()),
          axesArray(generateVertexArray()),
          axesBuffer(generateBuffer())
    {
        glBindVertexArray(vertexArray.getId());
        glBindBuffer(GL_ARRAY_BUFFER, centroidBuffer.getId());
        glVertexAttribPointer(CENTROID_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        setupOverlayVertexArray(controlPointsArray.getId(), controlPointsBuffer.getId());
        setupOverlayVertexArray(axesArray.getId(), axesBuffer.getId());

        retessellate();
        uploadControlPoints();
        uploadAxes();
    }

    BezierPatch::ControlPoints BezierPatch::loadControlPoints(const char *filename)
    {
        std::istringstream input(files::FileUtilities::getFileContents(filename));

        ControlPoints points{};
        for (int i = 0; i < CONTROL_POINT_COUNT; ++i)
        {
            if (!(input >> points[i].x >> points[i].y >> points[i].z))
            {
                throw std::runtime_error(
                    std::string(filename) + ": expected " + std::to_string(CONTROL_POINT_COUNT) +
                    " control points, got " + std::to_string(i)
                );
            }
        }

        return points;
    }

    glm::vec3 BezierPatch::computeCenter(const ControlPoints &points)
    {
        glm::vec3 sum{0.f};
        for (const auto &point : points)
            sum += point;
        return sum / static_cast<float>(points.size());
    }

    BezierPatch::SampleGrid BezierPatch::evaluate(const ControlPoints &points, const int resolution)
    {
        const int n = resolution;
        const float step = 1.f / static_cast<float>(n - 1);

        std::vector<std::array<float, ORDER>> basis(n);
        std::vector<std::array<float, ORDER>> derivative(n);
        for (int i = 0; i < n; ++i)
        {
            basis[i] = bernstein(static_cast<float>(i) * step);
            derivative[i] = bernsteinDerivative(static_cast<float>(i) * step);
        }

        SampleGrid grid{
            .size = n,
            .positions = std::vector<glm::vec3>(static_cast<std::size_t>(n) * n),
            .normals = std::vector<glm::vec3>(static_cast<std::size_t>(n) * n),
        };

        for (int row = 0; row < n; ++row)
        {
            const auto &bv = basis[row];
            const auto &dbv = derivative[row];

            for (int col = 0; col < n; ++col)
            {
                const auto &bu = basis[col];
                const auto &dbu = derivative[col];

                glm::vec3 point{0.f};
                glm::vec3 du{0.f};
                glm::vec3 dv{0.f};

                for (int j = 0; j < ORDER; ++j)
                {
                    for (int i = 0; i < ORDER; ++i)
                    {
                        const glm::vec3 &p = points[j * ORDER + i];
                        point += bu[i] * bv[j] * p;
                        du += dbu[i] * bv[j] * p;
                        dv += bu[i] * dbv[j] * p;
                    }
                }

                const glm::vec3 raw = glm::cross(du, dv);
                const float length = glm::length(raw);

                grid.positions[row * n + col] = point;
                grid.normals[row * n + col] = length > 1e-12f ? raw / length : glm::vec3(0.f, 0.f, 1.f);
            }
        }

        return grid;
    }

    BezierPatch::Tessellation BezierPatch::tessellateFlat(const SampleGrid &grid)
    {
        const int n = grid.size;
        const auto &p = grid.positions;

        Tessellation result;
        auto &data = result.vertexData;

        const std::size_t triangleCount = static_cast<std::size_t>(n - 1) * (n - 1) * 2;
        data.vertices.reserve(triangleCount * 9);
        data.normals.reserve(triangleCount * 9);
        data.elements.reserve(triangleCount * 3);
        result.centroids.reserve(triangleCount * 9);

        const auto addTriangle = [&](const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) {
            const glm::vec3 raw = glm::cross(b - a, c - a);
            const float length = glm::length(raw);
            const glm::vec3 normal = length > 1e-12f ? raw / length : glm::vec3(0.f, 0.f, 1.f);
            const glm::vec3 centroid = (a + b + c) / 3.f;

            for (const auto &v : {a, b, c})
            {
                appendVec3(data.vertices, v);
                appendVec3(data.normals, normal);
                appendVec3(result.centroids, centroid);
                data.elements.push_back(static_cast<GLuint>(data.elements.size()));
            }
        };

        for (int row = 0; row + 1 < n; ++row)
        {
            for (int col = 0; col + 1 < n; ++col)
            {
                const glm::vec3 &p00 = p[row * n + col];
                const glm::vec3 &p10 = p[row * n + col + 1];
                const glm::vec3 &p11 = p[(row + 1) * n + col + 1];
                const glm::vec3 &p01 = p[(row + 1) * n + col];

                addTriangle(p00, p10, p11);
                addTriangle(p00, p11, p01);
            }
        }

        return result;
    }

    BezierPatch::Tessellation BezierPatch::tessellateSmooth(const SampleGrid &grid)
    {
        const int n = grid.size;
        const float step = 1.f / static_cast<float>(n - 1);

        Tessellation result;
        auto &data = result.vertexData;

        const std::size_t vertexCount = static_cast<std::size_t>(n) * n;
        data.vertices.reserve(vertexCount * 3);
        data.normals.reserve(vertexCount * 3);
        data.texCoords.reserve(vertexCount * 2);
        data.elements.reserve(static_cast<std::size_t>(n - 1) * (n - 1) * 6);

        for (int row = 0; row < n; ++row)
        {
            for (int col = 0; col < n; ++col)
            {
                appendVec3(data.vertices, grid.positions[row * n + col]);
                appendVec3(data.normals, grid.normals[row * n + col]);
                appendVec2(data.texCoords, static_cast<float>(col) * step, static_cast<float>(row) * step);
            }
        }

        for (int row = 0; row + 1 < n; ++row)
        {
            for (int col = 0; col + 1 < n; ++col)
            {
                const auto i00 = static_cast<GLuint>(row * n + col);
                const auto i10 = static_cast<GLuint>(row * n + col + 1);
                const auto i11 = static_cast<GLuint>((row + 1) * n + col + 1);
                const auto i01 = static_cast<GLuint>((row + 1) * n + col);

                data.elements.insert(data.elements.end(), {i00, i10, i11, i00, i11, i01});
            }
        }

        return result;
    }

    void BezierPatch::retessellate()
    {
        const SampleGrid grid = evaluate(controlPoints, resolution);
        const bool flat = shadingMode == ShadingMode::Flat;

        const auto [data, centroids] = flat ? tessellateFlat(grid) : tessellateSmooth(grid);
        setVertexData(data, GL_DYNAMIC_DRAW);
        uploadCentroids(centroids);

        glBindVertexArray(vertexArray.getId());
        if (flat)
            glEnableVertexAttribArray(CENTROID_ATTRIBUTE);
        else
            glDisableVertexAttribArray(CENTROID_ATTRIBUTE);
        glBindVertexArray(0);
    }

    void BezierPatch::setShadingMode(const ShadingMode mode)
    {
        if (mode == shadingMode)
            return;

        shadingMode = mode;
        shaderProgram = mode == ShadingMode::Flat ? shaders.flat : shaders.textured;
        retessellate();
    }

    void BezierPatch::toggleShadingMode()
    {
        setShadingMode(shadingMode == ShadingMode::Flat ? ShadingMode::Textured : ShadingMode::Flat);
    }

    void BezierPatch::uploadCentroids(const std::vector<GLfloat> &centroids) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, centroidBuffer.getId());
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(centroids.size() * sizeof(GLfloat)),
            centroids.data(),
            GL_DYNAMIC_DRAW
        );
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BezierPatch::uploadControlPoints() const
    {
        std::vector<GLfloat> data;
        data.reserve(CONTROL_POINT_COUNT * 6);

        for (int i = 0; i < CONTROL_POINT_COUNT; ++i)
        {
            appendOverlayVertex(
                data,
                controlPoints[i],
                i == selectedIndex ? SELECTED_CONTROL_POINT_COLOR : CONTROL_POINT_COLOR
            );
        }

        glBindBuffer(GL_ARRAY_BUFFER, controlPointsBuffer.getId());
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(data.size() * sizeof(GLfloat)),
            data.data(),
            GL_DYNAMIC_DRAW
        );
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BezierPatch::uploadAxes() const
    {
        constexpr glm::vec3 origin{0.f};
        std::vector<GLfloat> data;
        data.reserve(6 * 6);

        appendOverlayVertex(data, origin, {1.f, 0.f, 0.f});
        appendOverlayVertex(data, {axisLength, 0.f, 0.f}, {1.f, 0.f, 0.f});
        appendOverlayVertex(data, origin, {0.f, 1.f, 0.f});
        appendOverlayVertex(data, {0.f, axisLength, 0.f}, {0.f, 1.f, 0.f});
        appendOverlayVertex(data, origin, {0.f, 0.f, 1.f});
        appendOverlayVertex(data, {0.f, 0.f, axisLength}, {0.f, 0.f, 1.f});

        glBindBuffer(GL_ARRAY_BUFFER, axesBuffer.getId());
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(data.size() * sizeof(GLfloat)),
            data.data(),
            GL_STATIC_DRAW
        );
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BezierPatch::setupOverlayVertexArray(const GLuint vertexArrayId, const GLuint bufferId)
    {
        glBindVertexArray(vertexArrayId);
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
            reinterpret_cast<const void *>(3 * sizeof(float))
        );
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BezierPatch::setOverlayUniforms(const RenderContext &context) const
    {
        const GLuint id = shaders.overlay.getId();
        glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, glm::value_ptr(context.model));
        glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, glm::value_ptr(context.view));
        glUniformMatrix4fv(glGetUniformLocation(id, "proj"), 1, GL_FALSE, glm::value_ptr(context.projection));
    }

    void BezierPatch::setSurfaceUniforms() const
    {
        const GLuint id = shaderProgram.getId();

        if (shadingMode == ShadingMode::Flat)
        {
            glUniform3fv(glGetUniformLocation(id, "diffuseColor"), 1, glm::value_ptr(diffuseColor));
            return;
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getId());
        glUniform1i(glGetUniformLocation(id, "diffuseTexture"), 0);

        glUniform1f(glGetUniformLocation(id, "ambientStrength"), ambientStrength);
        glUniform3fv(glGetUniformLocation(id, "specularColor"), 1, glm::value_ptr(specularColor));
        glUniform1f(glGetUniformLocation(id, "shininess"), shininess);
    }

    void BezierPatch::draw(const RenderContext &context) const
    {
        setSurfaceUniforms();
        Mesh::draw(context);
        glBindTexture(GL_TEXTURE_2D, 0);

        glUseProgram(shaders.overlay.getId());
        setOverlayUniforms(context);

        const GLint pointSizeLocation = glGetUniformLocation(shaders.overlay.getId(), "pointSize");

        glBindVertexArray(axesArray.getId());
        glDrawArrays(GL_LINES, 0, 6);

        glDisable(GL_DEPTH_TEST);

        glBindVertexArray(controlPointsArray.getId());
        glUniform1f(pointSizeLocation, pointSize);
        glDrawArrays(GL_POINTS, 0, CONTROL_POINT_COUNT);

        glUniform1f(pointSizeLocation, selectedPointSize);
        glDrawArrays(GL_POINTS, selectedIndex, 1);

        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(0);
    }

    void BezierPatch::selectNextControlPoint()
    {
        selectedIndex = (selectedIndex + 1) % CONTROL_POINT_COUNT;
        uploadControlPoints();
    }

    void BezierPatch::selectPreviousControlPoint()
    {
        selectedIndex = (selectedIndex + CONTROL_POINT_COUNT - 1) % CONTROL_POINT_COUNT;
        uploadControlPoints();
    }

    void BezierPatch::moveSelectedControlPoint(const glm::vec3 &delta)
    {
        controlPoints[selectedIndex] += delta;
        uploadControlPoints();
        retessellate();
    }

    void BezierPatch::setResolution(const int value)
    {
        const int clamped = std::clamp(value, MIN_RESOLUTION, MAX_RESOLUTION);
        if (clamped == resolution)
            return;

        resolution = clamped;
        retessellate();
    }
}
