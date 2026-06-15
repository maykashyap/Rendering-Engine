#include "engine/core/Execution.h"
#include "engine/core/IScript.h"
#include "engine/lib/matrix.h"
#include "engine/properties/MeshProperty.h"
#include "engine/properties/ShaderProperty.h"
#include "engine/properties/Transform.h"

using namespace Engine;

void Execution::pushScript(IScript *script) { m_ScriptStack.push_back(script); }

void Execution::Start() {
  for (auto i : m_ScriptStack) {
    i->Start();
  }
}

void Execution::Update() {
  while (!m_windowHandle->shouldClose()) {
    m_rendererHandle->clear(0.85, 0.85, 0.45, 1);
    m_rendererHandle->sceneStart(Math::mat4x4f::Identity());
    for (auto i : m_ScriptStack) {
      i->Update();
    }
    m_rendererHandle->sceneEnd();
    m_windowHandle->swapBuffers();
    m_windowHandle->pollEvents();
  }
}

void Execution::End() {
  for (auto i : m_ScriptStack) {
    i->End();
  }
  m_ScriptStack.clear();
}

// ___________________________________________________________________
// this is not opengl specific but i dont know where else to put this
void Execution::submitEntity(const Entity &entity) {
  auto *mesh = entity.getProperty<Property::Mesh>("Mesh");
  auto *transform = entity.getProperty<Property::Transform>("Transform");
  auto *shader = entity.getProperty<Property::ShaderProgram>("ShaderProgram");

  // Here you could check if the shader is null, you can use the global
  // shader.

  if (mesh && transform) {
    m_rendererHandle->submit({mesh->m_vahandle.get(),
                              shader->getShaderProgramHandle(),
                              &transform->transform});
  } else {
    throw std::runtime_error("what do you want me to do with this thing?");
  }
}
