#pragma once

#include "matrix.h"
#include <cmath>

namespace Engine::Math {
inline mat3x3f rotationMatrixEuler(float pitch_angle, float yaw_angle,
                                   float roll_angle) {
  // Y is up, x is right, and z is into the screen
  // in zx plane about y
  mat3x3f yaw({static_cast<float>(cos(yaw_angle)), 0,
               static_cast<float>(-sin(yaw_angle)), 0, 1, 0,
               static_cast<float>(sin(yaw_angle)), 0,
               static_cast<float>(cos(yaw_angle))});
  // in yz plane about x
  mat3x3f pitch({1, 0, 0, 0, static_cast<float>(cos(pitch_angle)),
                 static_cast<float>(-sin(pitch_angle)), 0,
                 static_cast<float>(sin(pitch_angle)),
                 static_cast<float>(cos(pitch_angle))});
  // in yx plane about z
  mat3x3f roll({static_cast<float>(cos(roll_angle)),
                static_cast<float>(sin(roll_angle)), 0,
                static_cast<float>(-sin(roll_angle)),
                static_cast<float>(cos(roll_angle)), 0, 0, 0, 1});

  return yaw * pitch * roll;
}

} // namespace Engine::Math
