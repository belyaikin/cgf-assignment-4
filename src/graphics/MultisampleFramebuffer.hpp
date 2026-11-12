//
// Created by Dmitriy on 12.11.2026.
//

#pragma once

#include "glad/gl.h"
#include "opengl/object/Framebuffer.hpp"
#include "opengl/object/Renderbuffer.hpp"

namespace assignment::graphics
{
    class MultisampleFramebuffer final
    {
        opengl::object::Framebuffer framebuffer;
        opengl::object::Renderbuffer colorBuffer;
        opengl::object::Renderbuffer depthBuffer;

        GLsizei samples;
        GLsizei width = 0;
        GLsizei height = 0;

    public:
        explicit MultisampleFramebuffer(GLsizei requestedSamples = 4);

        [[nodiscard]] GLsizei getSamples() const { return samples; }

        void resize(GLsizei newWidth, GLsizei newHeight);

        void bind() const;

        void resolveToDefaultFramebuffer() const;
    };
}
