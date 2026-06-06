#pragma once

namespace Engine {
class IScript {
public:
  virtual void Start() = 0;
  virtual void Update() = 0;
  virtual void End() = 0;
};
} // namespace Engine
