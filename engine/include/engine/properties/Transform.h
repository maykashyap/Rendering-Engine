#pragma once

#include "engine/core/IProperty.h"
#include "engine/lib/math3d.h"
#include "engine/lib/matrix.h"
#include "engine/lib/vector.h"
namespace Engine::Property {
using namespace Engine::Math;
class Transform : public IProperty {
private:
  mat4x4f transform;

public:
  vec3f translation;
  vec3f scale;
  vec3f rotation;
  Transform() : translation((vec3f)0), scale((vec3f)1), rotation((vec3f)0) {
    ID = "Transform";
  }
  Transform(const vec3f &translate, const vec3f &scale, const vec3f &rotate)
      : translation(translate), scale(scale), rotation(rotate) {
    ID = "Transform";
  }
  const mat4x4f *getTransformMatrix() {
    mat3x3f rotationMatrix =
        Math::rotationMatrixEuler(rotation.x, rotation.y, rotation.z);
    mat3x3f scaleMatrix({scale.x, 0, 0, 0, scale.y, 0, 0, 0, scale.z});
    transform = rotationMatrix * scaleMatrix;
    transform.setTranslation(translation);
    return &transform;
  }
};
} // namespace Engine::Property
