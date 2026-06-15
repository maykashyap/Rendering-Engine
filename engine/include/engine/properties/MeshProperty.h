#pragma once

#include "engine/core/BackendBuilder.h"
#include "engine/core/IProperty.h"
#include "engine/core/Mesh.h"
#include <string>
#include <string_view>
namespace Engine::Property {
class Mesh : public IProperty {
public:
  BackendBuilder::t_VAHandle m_vahandle;

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
};
} // namespace Engine::Property
