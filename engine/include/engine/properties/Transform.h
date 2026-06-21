#pragma once

#include "engine/core/IProperty.h"
#include "engine/lib/math3d.h"
#include "engine/lib/matrix.h"
#include "engine/lib/vector.h"
namespace Engine::Property {
using namespace Engine::Math;
class Transform : public IProperty {
private:
  mutable mat4x4f transform;

public:
  vec3f translation;
  vec3f scale;
  vec3f rotation;
  // Transform() : translation(0), scale(1), rotation(0) {};
  Transform(const vec3f &translate = (vec3f)0, const vec3f &scale = (vec3f)1,
            const vec3f &rotate = (vec3f)0)
      : translation(translate), scale(scale), rotation(rotate) {
    ID = "Transform";
  }
  const mat4x4f *getTransformMatrix() const {
    mat3x3f rotationMatrix =
        Math::rotationMatrixEuler(rotation.x, rotation.y, rotation.z);
    mat3x3f scaleMatrix({scale.x, 0, 0, 0, scale.y, 0, 0, 0, scale.z});
    transform = rotationMatrix * scaleMatrix;
    transform.setTranslation(translation);
    return &transform;
  }
};
} // namespace Engine::Property
