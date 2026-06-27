#pragma once

#include "engine/assets/IShader.h"
#include "engine/core/IRenderer.h"
#include "engine/window/IWindow.h"

#include "engine/assets/Mesh.h"
#include <memory>
#include <string_view>

// Could add preprocessors to select which version to use
// ie if i ever have multiple implementations
namespace Engine {
class BackendBuilder {
public:
  using t_Shader = std::shared_ptr<Assets::IShader>;
  using t_Window = std::unique_ptr<IWindow>;
  using t_Renderer = std::unique_ptr<Renderer::IRenderer>;
  using t_VAHandle = std::unique_ptr<Renderer::IGPUVertexArrayHandle>;

  static t_Window createWindow(int width, int height, std::string_view title);
  static t_Shader createShader(std::string_view vertexPath,
                               std::string_view fragmentPath);
  static t_VAHandle createVertexArrayHandle(const Assets::Mesh &mesh);
  static t_Renderer createRenderer();
};
} // namespace Engine
