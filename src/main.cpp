#include <iostream>

#include "glad/gl.h"

#include "Application.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/SmfLoader.hpp"
#include "graphics/opengl/object/shader/ShaderFactory.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 920
#define WINDOW_TITLE "Assignment 4 - Dmitriy Belyaikin"

using namespace assignment;

static void setupObjects(Application& application) {
    auto defaultVertexShader =
            graphics::opengl::object::shader::ShaderFactory::createShader(
            GL_VERTEX_SHADER,
            "../res/shaders/default.vert"
    );

    auto defaultFragmentShader =
            graphics::opengl::object::shader::ShaderFactory::createShader(
            GL_FRAGMENT_SHADER,
            "../res/shaders/default.frag"
    );

    const auto defaultShadersProgram =
            graphics::opengl::object::shader::ShaderFactory::createShaderProgram({
            defaultVertexShader, defaultFragmentShader
        }
    );

    defaultVertexShader.destroy();
    defaultFragmentShader.destroy();

    const auto model = graphics::Mesh{
        graphics::SmfLoader::toVertexData("../res/models/bunny.smf"),
        defaultShadersProgram,
        {0, 0, 0}
    };

    const auto whiteLightSource = graphics::LightSource{
        {-4, 2, 0},
        {0.5f, 0.f, 1.f, 1.f}
    };

    const auto redLightSource = graphics::LightSource{
        {4, 2, 0},
        {1.f, 0.f, 0.f, 1.f}
    };

    std::cout << "This model has " << model.getVertexData().vertices.size() << " vertices!\n";

    application.addMesh(model);
    application.addLightSource(whiteLightSource);
    application.addLightSource(redLightSource);
}

int main() {
    auto application = Application{
        {
            .width = WINDOW_WIDTH,
            .height = WINDOW_HEIGHT,
            .title = WINDOW_TITLE
        },
        graphics::Camera{
            glm::vec3(0.f, 0.f, -4.f)
        }
    };

    setupObjects(application);

    application.run();
    return 0;
}
