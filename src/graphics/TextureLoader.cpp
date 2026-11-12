//
// Created by Dmitriy on 12.11.2026.
//

#include "TextureLoader.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace assignment::graphics
{
    opengl::object::Texture TextureLoader::load2D(const char *filename)
    {
        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        unsigned char *pixels = stbi_load(filename, &width, &height, &channels, 0);

        if (!pixels)
            throw std::runtime_error(std::string(filename) + ": " + stbi_failure_reason());

        GLenum format;
        switch (channels)
        {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
            default:
                stbi_image_free(pixels);
                throw std::runtime_error(
                    std::string(filename) + ": unsupported channel count " + std::to_string(channels)
                );
        }

        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(
            GL_TEXTURE_2D, 0, static_cast<GLint>(format),
            width, height, 0,
            format, GL_UNSIGNED_BYTE, pixels
        );
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(pixels);

        std::cout << "[TextureLoader]: Loaded " << filename << " (" << width << "x" << height
                  << ", " << channels << " channels)\n";

        return opengl::object::Texture{id};
    }
}
