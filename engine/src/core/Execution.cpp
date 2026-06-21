#include "engine/core/Execution.h"
#include "engine/core/IScript.h"
#include "engine/lib/matrix.h"
#include "engine/properties/MeshProperty.h"
#include "engine/properties/ShaderProperty.h"
#include "engine/properties/Transform.h"
#include <iomanip>
#include <iostream>

using namespace Engine;

void Execution::Start() {
  IScript::executionHandle = this;
  for (const auto &i : m_ScriptStack) {
    i->Start();
  }
}

void Execution::Update() {
  while (!m_windowHandle->shouldClose()) {
    m_rendererHandle->clear(0.85, 0.85, 0.45, 1);
    m_rendererHandle->sceneStart(Math::mat4x4f::Identity());
    for (const auto &i : m_ScriptStack) {
      i->Update();
    }
    m_rendererHandle->sceneEnd();
    m_windowHandle->swapBuffers();
    m_windowHandle->pollEvents();
  }
}

void Execution::End() {
  for (const auto &i : m_ScriptStack) {
    i->End();
  }
  m_ScriptStack.clear();
}

// ___________________________________________________________________
void Execution::submitEntity(const Entity &entity) {
  auto *mesh = entity.getMesh();
  auto *shader = entity.getShader();

  // Here you could check if the shader is null, you can use the global
  // shader.

  if (mesh && shader) {
    m_rendererHandle->submit({mesh->m_vahandle.get(),
                              shader->getShaderProgramHandle(),
                              entity.getTransform().getTransformMatrix()});
  } else {
    throw std::runtime_error("what do you want me to do with this thing?");
  }
}
