#pragma once

#include "engine/assets/Mesh.h"
#include "engine/core/IRenderer.h"
#include "engine/lib/math/matrix.h"
#include "glad/glad.h"
#include <cstdint>

namespace Engine::Backend {
class OpenGLGPUVAHandle final : public Renderer::IGPUVertexArrayHandle {
private:
  GLuint m_vaoID = 0;
  GLuint m_vboID = 0;
  GLuint m_eboID = 0;
  uint32_t m_indexCount = 0;

public:
  OpenGLGPUVAHandle(const Assets::Mesh &mesh);
  ~OpenGLGPUVAHandle() override;

  void bind() const override;
  void unbind() const override;

  [[nodiscard]] uint32_t getIndexCount() const noexcept override {
    return m_indexCount;
  }

  // move and copy
};

class OpenGLRenderer final : public Renderer::IRenderer {
private:
  Math::mat4x4f m_viewProjection;
  std::vector<Renderer::RendererCommand> m_renderQueue;

  void flush();

public:
  OpenGLRenderer() = default;
  ~OpenGLRenderer() override = default;

  void init() override;
  void setViewport(uint32_t x, uint32_t y, uint32_t width,
                   uint32_t height) override;
  void clear(float r, float g, float b, float a) override;
  void sceneStart(const Math::mat4x4f &viewMatrix,
                  const Math::mat4x4f &projectionMatrix) override;
  void sceneEnd() override;
  void submit(const Renderer::RendererCommand &command) override;
};
} // namespace Engine::Backend
