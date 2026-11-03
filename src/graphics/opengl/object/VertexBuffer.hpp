//
// Created by Dmitriy on 02.10.2026.
//

#pragma once
#include "Object.hpp"

namespace assignment::graphics::opengl::object
{
    class VertexBuffer final : public Object
    {
    public:
        explicit VertexBuffer(const GLuint id)
            : Object(id)
        {
        }
    };
}
