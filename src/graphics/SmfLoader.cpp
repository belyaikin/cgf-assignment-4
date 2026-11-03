//
// Created by Dmitriy on 23.10.2026.
//

#include "SmfLoader.hpp"

#include <charconv>
#include <cstdint>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../files/FileUtilities.hpp"
#include "glm/geometric.hpp"
#include "glm/ext/vector_float3.hpp"

namespace assignment::graphics
{
    VertexData SmfLoader::toVertexData(const char *filename)
    {
        const auto contents = files::FileUtilities::getFileContents(filename);
        auto vertexData = VertexData{};
        std::istringstream input(contents);
        std::string line;
        std::size_t lineNumber = 0;

        while (std::getline(input, line))
        {
            ++lineNumber;
            std::istringstream lineInput(line);
            std::string record;
            if (!(lineInput >> record) || record.starts_with('#'))
            {
                continue;
            }

            if (record == "v")
            {
                GLfloat x;
                GLfloat y;
                GLfloat z;
                if (!(lineInput >> x >> y >> z))
                {
                    throw std::runtime_error(
                        std::string(filename) + ":" + std::to_string(lineNumber) +
                        ": vertex must contain three coordinates"
                    );
                }

                vertexData.vertices.insert(vertexData.vertices.end(), {x, y, z});
            }
            else if (record == "f")
            {
                std::vector<GLuint> face;
                std::string indexToken;
                while (lineInput >> indexToken)
                {
                    if (indexToken.starts_with('#'))
                    {
                        break;
                    }

                    std::uint64_t index;
                    const auto [end, error] = std::from_chars(
                        indexToken.data(),
                        indexToken.data() + indexToken.size(),
                        index
                    );
                    if (error != std::errc{} || end != indexToken.data() + indexToken.size() ||
                        index == 0 || index - 1 > std::numeric_limits<GLuint>::max())
                    {
                        throw std::runtime_error(
                            std::string(filename) + ":" + std::to_string(lineNumber) +
                            ": face indices must be positive 1-based integers"
                        );
                    }

                    face.push_back(static_cast<GLuint>(index - 1));
                }

                if (face.size() < 3)
                {
                    throw std::runtime_error(
                        std::string(filename) + ":" + std::to_string(lineNumber) +
                        ": face must contain at least three indices"
                    );
                }

                for (std::size_t i = 1; i + 1 < face.size(); ++i)
                {
                    vertexData.elements.insert(
                        vertexData.elements.end(),
                        {face[0], face[i], face[i + 1]}
                    );
                }
            }
        }

        const auto vertexCount = vertexData.vertices.size() / 3;
        for (const auto index : vertexData.elements)
        {
            if (index >= vertexCount)
            {
                throw std::runtime_error(
                    std::string(filename) + ": face index refers to a missing vertex"
                );
            }
        }

        const auto sourceVertices = std::move(vertexData.vertices);
        const auto sourceElements = std::move(vertexData.elements);
        vertexData.vertices.clear();
        vertexData.elements.clear();
        vertexData.normals.clear();

        const auto triangleCount = sourceElements.size() / 3;
        vertexData.vertices.reserve(triangleCount * 9);
        vertexData.elements.reserve(triangleCount * 3);
        vertexData.normals.reserve(triangleCount * 9);

        for (std::size_t tri = 0; tri < triangleCount; ++tri)
        {
            const GLuint i0 = sourceElements[tri * 3 + 0];
            const GLuint i1 = sourceElements[tri * 3 + 1];
            const GLuint i2 = sourceElements[tri * 3 + 2];

            const glm::vec3 a(sourceVertices[i0 * 3], sourceVertices[i0 * 3 + 1], sourceVertices[i0 * 3 + 2]);
            const glm::vec3 b(sourceVertices[i1 * 3], sourceVertices[i1 * 3 + 1], sourceVertices[i1 * 3 + 2]);
            const glm::vec3 c(sourceVertices[i2 * 3], sourceVertices[i2 * 3 + 1], sourceVertices[i2 * 3 + 2]);

            const glm::vec3 rawNormal = glm::cross(b - a, c - a);
            const float length = glm::length(rawNormal);
            const glm::vec3 faceNormal = length > 1e-12f
                ? rawNormal / length
                : glm::vec3(0.0f, 1.0f, 0.0f);

            for (const GLuint idx : {i0, i1, i2})
            {
                vertexData.vertices.insert(vertexData.vertices.end(), {
                    sourceVertices[idx * 3],
                    sourceVertices[idx * 3 + 1],
                    sourceVertices[idx * 3 + 2]
                });
                vertexData.normals.insert(vertexData.normals.end(), {
                    faceNormal.x, faceNormal.y, faceNormal.z
                });
                vertexData.elements.push_back(static_cast<GLuint>(vertexData.elements.size()));
            }
        }

        return vertexData;
    }
}
