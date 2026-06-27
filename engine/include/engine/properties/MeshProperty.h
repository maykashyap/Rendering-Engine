#pragma once

#include "engine/assets/Mesh.h"
#include "engine/core/BackendBuilder.h"
#include "engine/core/IRenderer.h"
#include "engine/lib/math/vector.h"
#include "engine/properties/IProperty.h"
#include <string>
#include <string_view>
namespace Engine::Property {
class Mesh : public IProperty {
private:
  BackendBuilder::t_VAHandle m_vahandle;
  Math::vec3f m_anchor = (Math::vec3f)0;

public:
  std::string name;
  std::string path;

  Mesh(std::string_view meshPath) : path(std::move(meshPath)) {
    ID = "Mesh";
    // Read mesh from file
  }
  Mesh(Assets::Mesh &mesh) {
    ID = "Mesh";
    m_vahandle = BackendBuilder::createVertexArrayHandle(mesh);
  }
  const Math::vec3f &getAnchor() const { return m_anchor; }
  void setAnchor(const Math::vec3f &anchor) { m_anchor = anchor; }
  Renderer::IGPUVertexArrayHandle *getVAHandle() { return m_vahandle.get(); }
};
} // namespace Engine::Property
