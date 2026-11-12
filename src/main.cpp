#include <iostream>
#include <memory>

#include "glad/gl.h"

#include "Application.hpp"
#include "graphics/BezierPatch.hpp"
#include "graphics/Camera.hpp"
#include "graphics/TextureLoader.hpp"
#include "graphics/opengl/object/shader/ShaderFactory.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 920
#define WINDOW_TITLE "Assignment 4 - Dmitriy Belyaikin"

#define CONTROL_POINT_STEP 0.1f

using namespace assignment;

static graphics::opengl::object::shader::ShaderProgram loadShaderProgram(
    const char *vertexShaderPath,
    const char *fragmentShaderPath
) {
    using graphics::opengl::object::shader::ShaderFactory;

    const auto vertexShader = ShaderFactory::createShader(GL_VERTEX_SHADER, vertexShaderPath);
    const auto fragmentShader = ShaderFactory::createShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

    const auto program = ShaderFactory::createShaderProgram({vertexShader, fragmentShader});

    vertexShader.destroy();
    fragmentShader.destroy();

    return program;
}

static void printControls() {
    std::cout
        << "[Controls]: Tab / Shift+Tab - select next / previous control point\n"
        << "[Controls]: L / J - move selected point +X / -X\n"
        << "[Controls]: I / K - move selected point +Y / -Y\n"
        << "[Controls]: O / U - move selected point +Z / -Z\n"
        << "[Controls]: = / - - increase / decrease patch resolution\n"
        << "[Controls]: M - toggle anti-aliasing\n"
        << "[Controls]: T - toggle flat shading / textured Phong shading\n";
}

static void bindPatchControls(Application &application, const std::shared_ptr<graphics::BezierPatch> &patch) {
    application.setKeyCallback([&application, patch](const int key, const int action, const int mods) {
        if (key == GLFW_KEY_M && action == GLFW_PRESS) {
            application.toggleAntiAliasing();
            return;
        }

        if (key == GLFW_KEY_T && action == GLFW_PRESS) {
            patch->toggleShadingMode();
            std::cout << "[Patch]: Shading mode "
                      << (patch->getShadingMode() == graphics::BezierPatch::ShadingMode::Flat ? "flat" : "textured Phong")
                      << "\n";
            return;
        }

        if (action != GLFW_PRESS && action != GLFW_REPEAT)
            return;

        glm::vec3 delta{0.f};

        switch (key) {
            case GLFW_KEY_TAB:
                if (mods & GLFW_MOD_SHIFT)
                    patch->selectPreviousControlPoint();
                else
                    patch->selectNextControlPoint();
                std::cout << "[Patch]: Selected control point " << patch->getSelectedIndex() << "\n";
                return;

            case GLFW_KEY_EQUAL:
            case GLFW_KEY_KP_ADD:
                patch->increaseResolution();
                std::cout << "[Patch]: Resolution " << patch->getResolution() << "\n";
                return;

            case GLFW_KEY_MINUS:
            case GLFW_KEY_KP_SUBTRACT:
                patch->decreaseResolution();
                std::cout << "[Patch]: Resolution " << patch->getResolution() << "\n";
                return;

            case GLFW_KEY_L: delta.x = CONTROL_POINT_STEP; break;
            case GLFW_KEY_J: delta.x = -CONTROL_POINT_STEP; break;
            case GLFW_KEY_I: delta.y = CONTROL_POINT_STEP; break;
            case GLFW_KEY_K: delta.y = -CONTROL_POINT_STEP; break;
            case GLFW_KEY_O: delta.z = CONTROL_POINT_STEP; break;
            case GLFW_KEY_U: delta.z = -CONTROL_POINT_STEP; break;

            default:
                return;
        }

        patch->moveSelectedControlPoint(delta);
    });
}

static void setupObjects(Application& application) {
    const auto patchShaderProgram = loadShaderProgram(
        "../res/shaders/patch.vert",
        "../res/shaders/patch.frag"
    );

    const auto texturedShaderProgram = loadShaderProgram(
        "../res/shaders/textured.vert",
        "../res/shaders/textured.frag"
    );

    const auto overlayShaderProgram = loadShaderProgram(
        "../res/shaders/overlay.vert",
        "../res/shaders/overlay.frag"
    );

    const auto controlPoints = graphics::BezierPatch::loadControlPoints("../res/models/patchPoints.txt");

    const auto wallTexture = graphics::TextureLoader::load2D("../res/images/wall.jpg");

    const auto patch = std::make_shared<graphics::BezierPatch>(
        controlPoints,
        graphics::BezierPatch::Shaders{
            .flat = patchShaderProgram,
            .textured = texturedShaderProgram,
            .overlay = overlayShaderProgram
        },
        wallTexture,
        -graphics::BezierPatch::computeCenter(controlPoints),
        12,
        graphics::BezierPatch::ShadingMode::Textured
    );

    const auto cameraLight = graphics::LightSource{
        {0.f, 1.f, 0.f},
        {1.f, 1.f, 1.f, 1.f},
        true
    };

    application.addMesh(patch);
    application.addLightSource(cameraLight);

    bindPatchControls(application, patch);
    printControls();
}

int main() {
    auto application = Application{
        {
            .width = WINDOW_WIDTH,
            .height = WINDOW_HEIGHT,
            .title = WINDOW_TITLE
        },
        graphics::Camera{
            glm::vec3(0.f, 0.f, -12.f)
        }
    };

    setupObjects(application);

    application.run();
    return 0;
}
