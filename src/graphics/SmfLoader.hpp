//
// Created by Dmitriy on 23.10.2026.
//
#pragma once

#include "Mesh.hpp"

namespace assignment::graphics
{
    class SmfLoader final
    {
    public:
        static VertexData toVertexData(const char *filename);
    };
}
