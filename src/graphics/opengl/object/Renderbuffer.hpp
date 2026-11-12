//
// Created by Dmitriy on 12.11.2026.
//

#pragma once
#include "Object.hpp"

namespace assignment::graphics::opengl::object
{
    class Renderbuffer final : public Object
    {
    public:
        explicit Renderbuffer(const GLuint id)
            : Object(id)
        {
        }
    };
}
