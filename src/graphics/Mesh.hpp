//
// Created by Dmitriy on 02.10.2026.
//

#pragma once
#include <vector>

#include "glad/gl.h"
#include "glm/vec3.hpp"
#include "opengl/object/ElementBuffer.hpp"
#include "opengl/object/VertexBuffer.hpp"
#include "opengl/object/VertexArray.hpp"
#include "opengl/object/shader/ShaderProgram.hpp"

namespace assignment::graphics
{
    struct VertexData {
        std::vector<GLfloat> vertices;
        std::vector<GLuint> elements;
        std::vector<GLfloat> normals;
    };

    class Mesh final
    {
        const opengl::object::shader::ShaderProgram shaderProgram;

        VertexData vertexData;

        const opengl::object::VertexBuffer vertexBuffer;
        const opengl::object::VertexBuffer normalsBuffer;
        const opengl::object::ElementBuffer elementBuffer;
        const opengl::object::VertexArray vertexArray;

        static GLuint generateVertexBuffer();
        static GLuint generateNormalsBuffer();
        static GLuint generateElementBuffer();
        static GLuint generateVertexArray();

        glm::vec3 position{};

    public:
        Mesh(const VertexData &vertexData, opengl::object::shader::ShaderProgram shaderProgram, glm::vec3 position);

        [[nodiscard]] const opengl::object::shader::ShaderProgram &getShaderProgram() const { return shaderProgram; }

        [[nodiscard]] const VertexData &getVertexData() const { return vertexData; }

        [[nodiscard]] const opengl::object::VertexBuffer &getVertexBuffer() const { return vertexBuffer; }
        [[nodiscard]] const opengl::object::ElementBuffer &getElementBuffer() const { return elementBuffer; }
        [[nodiscard]] const opengl::object::VertexArray &getVertexArray() const { return vertexArray; }

        glm::vec3 getPosition() const { return position; }
    };
}
