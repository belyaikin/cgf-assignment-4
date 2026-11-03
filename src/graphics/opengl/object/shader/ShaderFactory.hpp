//
// Created by Dmitriy on 20.09.2026.
//

#pragma once
#include <vector>

#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "glad/gl.h"

namespace assignment::graphics::opengl::object::shader
{
    class ShaderFactory
    {
    public:
        static Shader createShader(GLenum type, const char *sourcePath);
        static ShaderProgram createShaderProgram(const std::vector<Shader>& shaders);
    };
}