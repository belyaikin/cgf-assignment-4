//
// Created by Dmitriy on 02.10.2026.
//

#pragma once
#include <vector>

#include "glad/gl.h"
#include "glm/mat4x4.hpp"
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
        std::vector<GLfloat> texCoords;
    };

    struct RenderContext {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };

    class Mesh
    {
    protected:
        opengl::object::shader::ShaderProgram shaderProgram;

        VertexData vertexData;

        opengl::object::VertexBuffer vertexBuffer;
        opengl::object::VertexBuffer normalsBuffer;
        opengl::object::VertexBuffer texCoordsBuffer;
        opengl::object::ElementBuffer elementBuffer;
        opengl::object::VertexArray vertexArray;

        glm::vec3 position{};

        static GLuint generateBuffer();
        static GLuint generateVertexArray();

        void setVertexData(const VertexData &data, GLenum usage = GL_STATIC_DRAW);

    public:
        Mesh(const VertexData &vertexData, opengl::object::shader::ShaderProgram shaderProgram, glm::vec3 position);
        virtual ~Mesh() = default;

        virtual void draw(const RenderContext &context) const;

        [[nodiscard]] const opengl::object::shader::ShaderProgram &getShaderProgram() const { return shaderProgram; }

        [[nodiscard]] const VertexData &getVertexData() const { return vertexData; }

        [[nodiscard]] const opengl::object::VertexBuffer &getVertexBuffer() const { return vertexBuffer; }
        [[nodiscard]] const opengl::object::ElementBuffer &getElementBuffer() const { return elementBuffer; }
        [[nodiscard]] const opengl::object::VertexArray &getVertexArray() const { return vertexArray; }

        [[nodiscard]] glm::vec3 getPosition() const { return position; }
    };
}
