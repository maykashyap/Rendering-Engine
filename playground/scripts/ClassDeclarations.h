#pragma once
#include "engine.h"

using namespace Engine;

class Arm : public IScript {
  Entity lowerArm, upperArm, joint, tip, base, xAxis, yAxis;

  float time = 0, deltaTime = 0, speed = 0.2f;

  void Start() override;
  void Update() override;
  void End() override {};
};
