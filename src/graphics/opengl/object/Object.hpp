//
// Created by Dmitriy on 02.10.2026.
//

#pragma once
#include "glad/gl.h"

namespace assignment::graphics::opengl::object
{
    class Object
    {
    protected:
        GLuint id;

    public:
        explicit Object(const GLuint id) : id(id) {}

        [[nodiscard]] GLuint getId() const { return id; }
    };
}
