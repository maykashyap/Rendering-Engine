#include "engine/core/Engine.h"
#include "engine/core/IScript.h"
#include "engine/lib/matrix.h"

using namespace Engine;

void Execution::pushScript(IScript *script) { m_ScriptStack.push_back(script); }

void Execution::Start() {
  for (auto i : m_ScriptStack) {
    i->Start();
  }
}

void Execution::Update() {
  while (m_windowHandle->shouldClose()) {
    m_rendererHandle->sceneStart(Math::mat4x4f::Identity());
    m_rendererHandle->clear(0, 0, 0, 1);
    for (auto i : m_ScriptStack) {
      i->Update();
    }
    m_rendererHandle->sceneEnd();
  }
}

void Execution::End() {
  for (auto i : m_ScriptStack) {
    i->End();
  }
  m_ScriptStack.clear();
}
