//
// Created by Dmitriy on 20.09.2026.
//

#include "ShaderFactory.hpp"

#include <iostream>

#include "../../../../files/FileUtilities.hpp"
#include "glad/gl.h"

namespace assignment::graphics::opengl::object::shader
{
    Shader ShaderFactory::createShader(const GLenum type, const char *sourcePath) {
        const auto shader = Shader{glCreateShader(type)};

        const std::string source = files::FileUtilities::getFileContents(sourcePath);
        const GLchar* sourcePtr = source.c_str();
        glShaderSource(shader.getId(), 1, &sourcePtr, nullptr);

        glCompileShader(shader.getId());

        int success;
        char infoLog[512];
        glGetShaderiv(shader.getId(), GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader.getId(), 512, nullptr, infoLog);
            std::cout << "[ShaderFactory]: " << infoLog << "\n";
        }

        std::cout << "[ShaderFactory]: Successfully compiled shader " << shader.getId() << "\n";

        return shader;
    }

    ShaderProgram ShaderFactory::createShaderProgram(const std::vector<Shader>& shaders) {
        const auto shaderProgram = ShaderProgram{glCreateProgram()};

        std::cout << "[ShaderFactory]: Created shader program " << shaderProgram.getId() << "\n";

        for (const Shader& shader : shaders) {
            glAttachShader(shaderProgram.getId(), shader.getId());

            std::cout << "[ShaderFactory]: Attached shader " << shader.getId() << " to shader program " << shaderProgram.getId() << "\n";
        }

        glLinkProgram(shaderProgram.getId());

        return shaderProgram;
    }
}