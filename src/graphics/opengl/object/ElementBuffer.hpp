//
// Created by Dmitriy on 23.10.26.
//

#pragma once
#include "Object.hpp"

namespace assignment::graphics::opengl::object {
    class ElementBuffer final : public Object {
    public:
        explicit ElementBuffer(const GLuint id)
            : Object(id)
        {
        }
    };
}
