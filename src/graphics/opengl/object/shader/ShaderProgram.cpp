//
// Created by Dmitriy on 18.10.26.
//

#include "ShaderProgram.hpp"

namespace assignment::graphics::opengl::object::shader {
    void ShaderProgram::setBool(const std::string &name, const bool value) {
        const GLuint uniformId = glGetUniformLocation(this->id, name.c_str());
        glUniform1i(uniformId, value);
    }

    void ShaderProgram::setInt(const std::string &name, const int value) {
        const GLuint uniformId = glGetUniformLocation(this->id, name.c_str());
        glUniform1i(uniformId, value);
    }

    void ShaderProgram::setFloat(const std::string &name, const float value)
    {
        const GLuint uniformId = glGetUniformLocation(this->id, name.c_str());
        glUniform1f(uniformId, value);
    }
}
