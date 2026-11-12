//
// Created by Dmitriy on 12.11.2026.
//

#pragma once
#include "Object.hpp"

namespace assignment::graphics::opengl::object
{
    class Framebuffer final : public Object
    {
    public:
        explicit Framebuffer(const GLuint id)
            : Object(id)
        {
        }
    };
}
