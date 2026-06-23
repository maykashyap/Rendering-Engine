#pragma once

#include "engine/lib/matrix.h"
#include "engine/renderer/IShader.h"
#include "glad/glad.h"
#include <string_view>

namespace Engine::Backend {
class OpenGLShader final : public Assets::IShader {
private:
  GLuint ID;

public:
  OpenGLShader(std::string_view vertexPath, std::string_view fragmentPath);
  ~OpenGLShader() override;

  OpenGLShader(OpenGLShader &&other) noexcept;
  OpenGLShader &operator=(OpenGLShader &&other) noexcept;

  void use() const override;
  void setUniform(std::string_view name, bool val) const override;
  void setUniform(std::string_view name, int val) const override;
  void setUniform(std::string_view name, float val) const override;
  void setUniform(std::string_view name,
                  const Math::mat4x4f *matrix) const override;
  void setUniform(std::string_view name,
                  const Math::vec3f *vec3) const override;
};
} // namespace Engine::Backend
