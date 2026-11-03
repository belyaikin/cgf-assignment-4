//
// Created by Dmitriy on 02.10.2026.
//

#include "Mesh.hpp"

#include "glm/vec3.hpp"
#include "opengl/object/shader/ShaderProgram.hpp"

namespace assignment::graphics
{
    Mesh::Mesh(const VertexData &vertexData, const opengl::object::shader::ShaderProgram shaderProgram, glm::vec3 position)
        : shaderProgram(shaderProgram),
          vertexData(vertexData),
          vertexBuffer(generateVertexBuffer()),
          normalsBuffer(generateNormalsBuffer()),
          elementBuffer(generateElementBuffer()),
          vertexArray(generateVertexArray()), position(position) {
        // Vertex Array Object initialization should be somewhere else but not here
        glBindVertexArray(vertexArray.getId());
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.getId());

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(vertexData.vertices.size() * sizeof(float)),
            vertexData.vertices.data(),
            GL_STATIC_DRAW
        );

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            nullptr
        );
        glEnableVertexAttribArray(0);

        if (!vertexData.normals.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer.getId());
            glBufferData(
                GL_ARRAY_BUFFER,
                static_cast<GLsizeiptr>(vertexData.normals.size() * sizeof(float)),
                vertexData.normals.data(),
                GL_STATIC_DRAW
            );

            glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                3 * sizeof(float),
                nullptr
            );
            glEnableVertexAttribArray(1);
        }

        if (!vertexData.elements.empty()) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer.getId());
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                static_cast<GLsizeiptr>(vertexData.elements.size() * sizeof(GLuint)),
                vertexData.elements.data(),
                GL_STATIC_DRAW
            );
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLuint Mesh::generateVertexArray()
    {
        GLuint vertexArrayId;
        glGenVertexArrays(1, &vertexArrayId);
        return vertexArrayId;
    }

    GLuint Mesh::generateVertexBuffer()
    {
        GLuint vertexBufferId;
        glGenBuffers(1, &vertexBufferId);
        return vertexBufferId;
    }


    GLuint Mesh::generateNormalsBuffer() {
        GLuint normalsBufferId;
        glGenBuffers(1, &normalsBufferId);
        return normalsBufferId;
    }

    GLuint Mesh::generateElementBuffer() {
        GLuint elementBufferId;
        glGenBuffers(1, &elementBufferId);
        return elementBufferId;
    }
}
