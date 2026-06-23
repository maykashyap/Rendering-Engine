#pragma once

#include "engine/core/BackendBuilder.h"
#include "engine/core/IProperty.h"
#include "engine/renderer/IShader.h"
#include <string_view>
namespace Engine::Property {
class ShaderProgram : public IProperty {
private:
  BackendBuilder::t_Shader m_shader = nullptr;
  std::string vertexPath, fragmentPath;

public:
  ShaderProgram(BackendBuilder::t_Shader shader) {
    ID = "ShaderProgram";
    m_shader = std::move(shader);
  }
  std::string_view getVertexPath() const { return vertexPath; }
  std::string_view getFragmentPath() const { return fragmentPath; }

  const Assets::IShader *getShaderProgramHandle() const {
    return m_shader.get();
  }
};
} // namespace Engine::Property
