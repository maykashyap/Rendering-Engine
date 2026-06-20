#pragma once

#include "engine/core/IProperty.h"
#include "engine/renderer/IShader.h"
#include <string_view>
namespace Engine::Property {
class ShaderProgram : public IProperty {
private:
  const Assets::IShader *m_shader = nullptr;
  std::string vertexPath, fragmentPath;

public:
  ShaderProgram(Assets::IShader *shader) {
    ID = "ShaderProgram";
    m_shader = shader;
  }
  std::string_view getVertexPath() const { return vertexPath; }
  std::string_view getFragmentPath() const { return fragmentPath; }

  const Assets::IShader *getShaderProgramHandle() const { return m_shader; }
};
} // namespace Engine::Property
