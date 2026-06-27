// src/backends/opengl/OpenGLTexture.h
#pragma once
#include "engine/assets/ITexture.h"
#include "glad/glad.h"
#include <string_view>

namespace Engine::Backend {

class OpenGLTexture final : public Assets::ITexture {
private:
  GLuint m_id;

public:
  explicit OpenGLTexture(std::string_view path);
  ~OpenGLTexture() override;

  void bind(uint32_t unit) const override;
};

} // namespace Engine::Backend
