//
// Created by Dmitriy on 02.10.2026.
//

#pragma once
#include "Object.hpp"

namespace assignment::graphics::opengl::object
{
    class VertexArray : public Object
    {
    public:
        explicit VertexArray(GLuint id)
            : Object(id)
        {
        }
    };
}
