//
// Created by Dmitriy on 12.11.2026.
//

#include "MultisampleFramebuffer.hpp"

#include <algorithm>
#include <iostream>

namespace assignment::graphics
{
    namespace
    {
        GLuint generateFramebuffer()
        {
            GLuint id;
            glGenFramebuffers(1, &id);
            return id;
        }

        GLuint generateRenderbuffer()
        {
            GLuint id;
            glGenRenderbuffers(1, &id);
            return id;
        }

        GLsizei clampSamples(const GLsizei requested)
        {
            GLint maxSamples = 1;
            glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
            return std::clamp(requested, 1, maxSamples);
        }
    }

    MultisampleFramebuffer::MultisampleFramebuffer(const GLsizei requestedSamples)
        : framebuffer(generateFramebuffer()),
          colorBuffer(generateRenderbuffer()),
          depthBuffer(generateRenderbuffer()),
          samples(clampSamples(requestedSamples))
    {
    }

    void MultisampleFramebuffer::resize(const GLsizei newWidth, const GLsizei newHeight)
    {
        if (newWidth == width && newHeight == height)
            return;

        width = newWidth;
        height = newHeight;

        glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer.getId());
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, width, height);

        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer.getId());
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.getId());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer.getId());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer.getId());

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "[MultisampleFramebuffer]: Framebuffer is incomplete\n";

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void MultisampleFramebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.getId());
    }

    void MultisampleFramebuffer::resolveToDefaultFramebuffer() const
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.getId());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
