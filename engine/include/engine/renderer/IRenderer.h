// Fruitcakes

#pragma once

#include "engine/lib/matrix.h"
#include "engine/lib/vector.h"
#include "engine/properties/Transform.h"
#include "engine/renderer/IShader.h"
#include <cstdint>
namespace Engine::Renderer {
class IGPUVertexArrayHandle {
public:
  virtual ~IGPUVertexArrayHandle() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual uint32_t getIndexCount() const = 0;
};

struct RendererCommand {
  IGPUVertexArrayHandle *vaHandle;
  const Assets::IShader *shader;
  const Property::Transform *translation; // at this stage of the code I doubt
                                          // you would/should alter this matrix.
  const Math::vec3f *anchor;
};

class IRenderer {
public:
  IRenderer() = default;
  virtual ~IRenderer() = default;

  IRenderer(const IRenderer &) = delete;
  IRenderer &operator=(const IRenderer &) = delete;

  virtual void init() = 0;
  virtual void setViewport(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) = 0;
  virtual void clear(float r, float g, float b, float a) = 0;
  virtual void sceneStart(const Math::mat4x4f &projectionMatrix) = 0;
  virtual void sceneEnd() = 0;
  virtual void submit(const RendererCommand &command) = 0;
};
} // namespace Engine::Renderer
