#pragma once

#include "engine/renderer/IShader.h"
#include "glad/glad.h"
#include <string_view>

namespace Engine::Backend {
class OpenGLShader final : public IShader {
private:
  GLuint ID;

public:
  OpenGLShader(std::string_view vertexPath, std::string_view fragmentPath);
  ~OpenGLShader() override;

  OpenGLShader(OpenGLShader &&other) noexcept;
  OpenGLShader &operator=(OpenGLShader &&other) noexcept;

  void use() const override;
  void setBool(std::string_view name, bool val) const override;
  void setInt(std::string_view name, int val) const override;
  void setFloat(std::string_view name, float val) const override;
};
} // namespace Engine::Backend
