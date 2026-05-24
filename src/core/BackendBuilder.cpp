#include "engine/core/BackendBuilder.h"
#include "../backends/glfw/GLFWWindow.h"
#include "../backends/opengl/OpenGLShader.h"
#include "engine/renderer/IShader.h"
#include "engine/window/IWindow.h"
#include <memory>
using namespace Engine;

std::unique_ptr<IWindow> BackendBuilder::createWindow(int width, int height,
                                                      std::string_view title) {
  return std::make_unique<Backend::GLFWWindow>(width, height, title);
}

std::unique_ptr<IShader>
BackendBuilder::createShader(std::string_view vertexPath,
                             std::string_view fragmentPath) {
  return std::make_unique<Backend::OpenGLShader>(vertexPath, fragmentPath);
}
