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
          vertexBuffer(generateBuffer()),
          normalsBuffer(generateBuffer()),
          texCoordsBuffer(generateBuffer()),
          elementBuffer(generateBuffer()),
          vertexArray(generateVertexArray()),
          position(position)
    {
        glBindVertexArray(vertexArray.getId());

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.getId());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer.getId());
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, texCoordsBuffer.getId());
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer.getId());

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        setVertexData(vertexData);
    }

    void Mesh::setVertexData(const VertexData &data, const GLenum usage)
    {
        vertexData = data;

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.getId());
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(vertexData.vertices.size() * sizeof(GLfloat)),
            vertexData.vertices.data(),
            usage
        );

        glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer.getId());
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(vertexData.normals.size() * sizeof(GLfloat)),
            vertexData.normals.data(),
            usage
        );

        glBindBuffer(GL_ARRAY_BUFFER, texCoordsBuffer.getId());
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(vertexData.texCoords.size() * sizeof(GLfloat)),
            vertexData.texCoords.data(),
            usage
        );
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(vertexArray.getId());

        // Optional attributes are only read when they actually have data.
        if (vertexData.normals.empty())
            glDisableVertexAttribArray(1);
        else
            glEnableVertexAttribArray(1);

        if (vertexData.texCoords.empty())
            glDisableVertexAttribArray(2);
        else
            glEnableVertexAttribArray(2);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(vertexData.elements.size() * sizeof(GLuint)),
            vertexData.elements.data(),
            usage
        );
        glBindVertexArray(0);
    }

    void Mesh::draw(const RenderContext &) const
    {
        glBindVertexArray(vertexArray.getId());

        if (!vertexData.elements.empty()) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexData.elements.size()), GL_UNSIGNED_INT, nullptr);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexData.vertices.size() / 3));
        }

        glBindVertexArray(0);
    }

    GLuint Mesh::generateVertexArray()
    {
        GLuint vertexArrayId;
        glGenVertexArrays(1, &vertexArrayId);
        return vertexArrayId;
    }

    GLuint Mesh::generateBuffer()
    {
        GLuint bufferId;
        glGenBuffers(1, &bufferId);
        return bufferId;
    }
}
