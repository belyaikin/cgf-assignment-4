cmake_minimum_required(VERSION 4.1)
project(assignment4)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.5.1
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

include_directories("deps/glad/include" "deps/stb")

add_executable(${PROJECT_NAME}
        deps/glad/src/gl.c
        src/main.cpp
        src/graphics/opengl/object/shader/ShaderFactory.cpp
        src/graphics/opengl/object/shader/ShaderFactory.hpp
        src/graphics/opengl/object/shader/Shader.hpp
        src/graphics/opengl/object/Object.hpp
        src/graphics/opengl/object/shader/ShaderProgram.hpp
        src/graphics/window/WindowUtilities.hpp
        src/graphics/Renderer.cpp
        src/graphics/Renderer.hpp
        src/graphics/Mesh.cpp
        src/graphics/Mesh.hpp
        src/graphics/opengl/object/VertexBuffer.hpp
        src/graphics/opengl/object/VertexArray.hpp
        src/Application.hpp
        src/Application.cpp
        src/graphics/opengl/object/shader/Shader.cpp
        src/graphics/window/WindowUtilities.cpp
        src/files/FileUtilities.hpp
        src/files/FileUtilities.cpp
        src/graphics/opengl/object/shader/ShaderProgram.cpp
        src/graphics/Camera.hpp
        src/graphics/Camera.cpp
        src/graphics/opengl/object/ElementBuffer.hpp
        src/graphics/SmfLoader.hpp
        src/graphics/SmfLoader.cpp
        src/graphics/LightSource.hpp
        src/graphics/LightSource.cpp
        src/graphics/BezierPatch.hpp
        src/graphics/BezierPatch.cpp
        src/graphics/MultisampleFramebuffer.hpp
        src/graphics/MultisampleFramebuffer.cpp
        src/graphics/opengl/object/Framebuffer.hpp
        src/graphics/opengl/object/Renderbuffer.hpp
        src/graphics/opengl/object/Texture.hpp
        src/graphics/TextureLoader.hpp
        src/graphics/TextureLoader.cpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        OpenGL::GL
        glfw
        glm::glm
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        "deps/glad/include"
        "deps/stb"
)