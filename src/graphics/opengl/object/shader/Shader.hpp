//
// Created by Dmitriy on 02.10.2026.
//

#pragma once
#include <string>

#include "../Object.hpp"
#include "glad/gl.h"

namespace assignment::graphics::opengl::object::shader
{
    class Shader : public Object
    {
    public:
        explicit Shader(GLuint id);

        void destroy() const;
    };
}
