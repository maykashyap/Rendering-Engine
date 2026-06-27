#pragma once

#include "engine/core/Entity.h"
#include "engine/core/IRenderer.h"
#include "engine/core/IScript.h"
#include "engine/properties/CameraProperty.h"
#include "engine/window/IWindow.h"
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>
namespace Engine {
class IScript;
class Execution {
public:
  using t_Script = std::unique_ptr<IScript>;

  Execution(Renderer::IRenderer &ra, IWindow &wa)
      : m_rendererHandle(&ra), m_windowHandle(&wa) {}
  ~Execution() = default;
  void registerEntity(const Entity &entity) {
    m_renderables.push_back(&entity);
  }
  template <typename T, typename... Args> void injectScript(Args &&...args) {
    static_assert(std::is_base_of_v<IScript, T>,
                  "Injectable scripts must inherit Engine::IScript.");
    auto script = std::make_unique<T>(std::forward<Args>(args)...);
    script->executionHandle = this;
    m_ScriptStack.push_back(std::move(script));
  }
  template <typename T> T *getScript() {
    for (const auto &script : m_ScriptStack) {
      // null if cast failed.
      T *derived = dynamic_cast<T *>(script.get());
      if (derived) {
        return derived;
      }
    }
    throw std::runtime_error("Invalid script invoked.");
  }
  Renderer::IRenderer *getRendererHandle() { return m_rendererHandle; }
  IWindow *getWindowHandle() { return m_windowHandle; }

  void submitEntity(const Entity &entity);
  void attachCamera(Property::CameraProperty &cameraProperty) {
    m_cameraHandle = &cameraProperty;
  }

  void Start();
  void Update();
  void End();

private:
  std::vector<const Entity *> m_renderables;
  Property::CameraProperty *m_cameraHandle = nullptr;
  Renderer::IRenderer *m_rendererHandle;
  IWindow *m_windowHandle;
  std::vector<t_Script> m_ScriptStack;
};
} // namespace Engine
