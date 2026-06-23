#pragma once

#include "engine/core/BackendBuilder.h"
#include "engine/core/IProperty.h"
#include "engine/core/Mesh.h"
#include "engine/lib/vector.h"
#include "engine/renderer/IRenderer.h"
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

  Mesh(std::string_view meshName, std::string_view meshPath)
      : name(std::move(meshName)), path(std::move(meshPath)) {
    ID = "Mesh";
    // Read mesh from file
  }
  Mesh(std::string_view meshName, Assets::Mesh &mesh)
      : name(std::move(meshName)), path("") {
    ID = "Mesh";
    m_vahandle = BackendBuilder::createVertexArrayHandle(mesh);
  }
  const Math::vec3f &getAnchor() const { return m_anchor; }
  void setAnchor(const Math::vec3f &anchor) { m_anchor = anchor; }
  Renderer::IGPUVertexArrayHandle *getVAHandle() { return m_vahandle.get(); }
};
} // namespace Engine::Property
