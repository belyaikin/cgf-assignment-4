//
// Created by Dmitriy on 12.11.2026.
//

#pragma once

#include "opengl/object/Texture.hpp"

namespace assignment::graphics
{
    class TextureLoader final
    {
    public:
        static opengl::object::Texture load2D(const char *filename);
    };
}
