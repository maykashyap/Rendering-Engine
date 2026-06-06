#include "engine/properties/MeshProperty.h"
#include "engine/core/BackendBuilder.h"
#include "engine/properties/ShaderProperty.h"
#include "engine/properties/Transform.h"

using namespace Engine;
void Property::Mesh::Start() {
  m_vahandle = BackendBuilder::createVertexArrayHandle(meshData);
}

void Property::Mesh::Update() {
  m_rendererHandle->submit(
      {m_vahandle.get(),
       m_owner->getProperty<ShaderProgram>("Shader")->getShaderProgramHandle(),
       &m_owner->getProperty<Transform>("Transform")->transform});
}

void Property::Mesh::End() {
  // everything should delete itself idk
}
