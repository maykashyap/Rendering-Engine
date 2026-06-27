#include "engine/core/Execution.h"
#include "engine/core/IScript.h"
#include "engine/properties/CameraProperty.h"
#include "engine/properties/MeshProperty.h"
#include "engine/properties/ShaderProperty.h"
#include "engine/properties/Transform.h"
#include <stdexcept>

using namespace Engine;

void Execution::Start() {
  for (const auto &i : m_ScriptStack) {
    i->Start();
  }
}

void Execution::Update() {
  while (!m_windowHandle->shouldClose()) {
    m_rendererHandle->clear(0.85, 0.85, 0.45, 1);
    if (!m_cameraHandle)
      throw std::runtime_error("No camera attached to execution.");
    // Needs to happen before entity submission.
    // and currently also before view calculation.
    for (const Entity *entity : m_renderables)
      entity->getTransform().markDirty();
    m_cameraHandle->setView();
    m_rendererHandle->sceneStart(m_cameraHandle->getViewMatrix(),
                                 m_cameraHandle->getProjectionMatrix());
    for (const auto &i : m_ScriptStack)
      i->Update();
    for (const Entity *entity : m_renderables) {
      if (entity->isEnabled)
        submitEntity(*entity);
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
    m_rendererHandle->submit({mesh->getVAHandle(),
                              shader->getShaderProgramHandle(),
                              &entity.getTransform(), &mesh->getAnchor()});
  } else {
    throw std::runtime_error("what do you want me to do with this thing?");
  }
}
