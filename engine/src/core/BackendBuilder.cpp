#include "engine/core/BackendBuilder.h"

#include "../backends/opengl/OpenGLRenderer.h"
#include "../backends/opengl/OpenGLShader.h"

#include "../backends/glfw/GLFWWindow.h"

#include <memory>

using namespace Engine;

std::unique_ptr<IWindow> BackendBuilder::createWindow(int width, int height,
                                                      std::string_view title) {
  return std::make_unique<Backend::GLFWWindow>(width, height, title);
}

BackendBuilder::t_Shader
BackendBuilder::createShader(std::string_view vertexPath,
                             std::string_view fragmentPath) {
  return std::make_shared<Backend::OpenGLShader>(vertexPath, fragmentPath);
}

BackendBuilder::t_VAHandle
BackendBuilder::createVertexArrayHandle(const Assets::Mesh &mesh) {
  return std::make_unique<Backend::OpenGLGPUVAHandle>(mesh);
}

BackendBuilder::t_Renderer BackendBuilder::createRenderer() {
  return std::make_unique<Backend::OpenGLRenderer>();
}
