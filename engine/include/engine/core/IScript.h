#pragma once

#include "engine/core/Execution.h"
namespace Engine {
class Execution;
class IScript {
public:
  inline static Engine::Execution *executionHandle = nullptr;
  virtual void Start() = 0;
  virtual void Update() = 0;
  virtual void End() = 0;
};
} // namespace Engine
