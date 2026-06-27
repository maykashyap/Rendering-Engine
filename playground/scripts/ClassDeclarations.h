#pragma once
#include "engine.h"

using namespace Engine;

class Arm : public IScript {
  Entity lowerArm, upperArm, joint, tip, base, xAxis, yAxis, camera;

  float time = 0, deltaTime = 0, speed = 0.2f;

  void Start() override;
  void Update() override;
  void End() override {};
};

class Cube : public IScript {
  Entity cube, camera;
  float time = 0, deltaTime = 0;

  void Start() override;
  void Update() override;
  void End() override {};
};
