#pragma once

#include "engine/core/BackendBuilder.h"
#include "engine/core/IProperty.h"
#include "engine/core/IScript.h"
#include "engine/core/Mesh.h"
#include "engine/renderer/IRenderer.h"
#include <string>
#include <string_view>
namespace Engine::Property {
class Mesh : IScript, IProperty {
private:
  BackendBuilder::t_VAHandle m_vahandle;
  BackendBuilder::t_Renderer m_rendererHandle;
  Renderer::RendererCommand m_renderCommand = {};
  Assets::Mesh meshData;

public:
  std::string name;
  std::string path;

  Mesh(std::string_view meshName, std::string_view meshPath)
      : name(std::move(meshName)), path(std::move(meshPath)) {
    ID = "Mesh";
  }
  Mesh(std::string_view meshName, Assets::Mesh mesh)
      : meshData(std::move(mesh)), name(std::move(meshName)), path("") {}

  void Start() override;
  void Update() override;
  void End() override;
};
} // namespace Engine::Property
