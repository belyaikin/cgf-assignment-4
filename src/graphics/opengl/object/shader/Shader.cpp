//
// Created by Dmitriy on 17.10.2026.
//

#include "Shader.hpp"

#include <fstream>
#include <ios>

namespace assignment::graphics::opengl::object::shader
{
    Shader::Shader(const GLuint id) : Object(id) {}

    void Shader::destroy() const
    {
        glDeleteShader(this->id);
    }
}
