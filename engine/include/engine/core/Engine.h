#pragma once

#include "engine/core/IScript.h"
#include "engine/renderer/IRenderer.h"
#include "engine/window/IWindow.h"
#include <memory>
#include <vector>
namespace Engine {
class Execution {
public:
  using t_Script = std::unique_ptr<IScript>;

  Execution(Renderer::IRenderer *ra, IWindow *wa)
      : m_rendererHandle(ra), m_windowHandle(wa) {}
  ~Execution();

  void pushScript(IScript *);
  Renderer::IRenderer *getRendererHandle() { return m_rendererHandle; }

  void Start();
  void Update();
  void End();

private:
  Renderer::IRenderer *m_rendererHandle;
  IWindow *m_windowHandle;
  std::vector<IScript *> m_ScriptStack;
};
} // namespace Engine
