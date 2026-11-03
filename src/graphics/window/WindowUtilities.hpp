//
// Created by Dmitriy on 02.10.2026.
//

#pragma once
#include <iostream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

namespace assignment::graphics::window
{
    class WindowUtilities
    {
    public:
        static void windowSizeChangeCallback(GLFWwindow *window, int width, int height);

        static GLFWwindow * createWindow(int width, int height, const char* title);
    };
}
