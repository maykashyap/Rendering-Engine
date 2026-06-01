// Fruitcakes

#pragma once

#include "engine/lib/matrix.h"
#include "engine/renderer/IShader.h"
#include <cstdint>
#include <memory>
namespace Engine::Renderer {
class IGPUVertexArrayHandle {
public:
  virtual ~IGPUVertexArrayHandle() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual uint32_t getIndexCount() const = 0;
};

struct RendererCommand {
  std::shared_ptr<IGPUVertexArrayHandle> vaHandle;
  std::shared_ptr<IShader> shader;
  // A Transform Matrix that will be passed to the shader per object.
};

class IRenderer {
public:
  virtual ~IRenderer() = default;

  IRenderer(const IRenderer &) = delete;
  IRenderer &operator=(const IRenderer &) = delete;

  virtual void init() = 0;
  virtual void setViewport(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) = 0;
  virtual void clear(float r, float g, float b, float a) = 0;
  virtual void
      frameStart(/* here is where you provide your projection matrix*/) = 0;
  virtual void frameEnd() = 0;
  virtual void submit(const RendererCommand &command) = 0;
};
} // namespace Engine::Renderer
