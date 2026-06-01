#pragma once

#include "engine/renderer/IRenderer.h"
#include "engine/renderer/IShader.h"
#include "engine/window/IWindow.h"

#include "engine/core/Mesh.h"
#include <memory>
#include <string_view>

// Could add preprocessors to select which version to use
// ie if i ever have multiple implementations
namespace Engine {
class BackendBuilder {
public:
  static std::unique_ptr<IWindow> createWindow(int width, int height,
                                               std::string_view title);
  static std::unique_ptr<IShader> createShader(std::string_view vertexPath,
                                               std::string_view fragmentPath);
  static std::unique_ptr<Renderer::IGPUVertexArrayHandle>
  createVertexArrayHandle(const Assets::Mesh &mesh);
};
} // namespace Engine
