#pragma once

#include "engine/core/BackendBuilder.h"
#include "engine/core/IProperty.h"
#include "engine/renderer/IShader.h"
#include <string_view>
namespace Engine::Property {
class ShaderProgram : IProperty {
private:
  BackendBuilder::t_Shader shader;
  std::string vertexPath, fragmentPath;

public:
  ShaderProgram(std::string_view vertexShaderPath,
                std::string_view fragmentShaderPath)
      : vertexPath(std::move(vertexShaderPath)),
        fragmentPath(std::move(fragmentShaderPath)) {
    ID = "ShaderProgram";
    shader = BackendBuilder::createShader(vertexShaderPath, fragmentShaderPath);
  }
  std::string_view getVertexPath() const { return vertexPath; }
  std::string_view getFragmentPath() const { return fragmentPath; }

  IShader *getShaderProgramHandle() { return shader.get(); }
};
} // namespace Engine::Property
