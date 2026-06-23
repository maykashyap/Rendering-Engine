#pragma once

#include "engine/core/IProperty.h"
#include "engine/lib/matrix.h"
#include "engine/lib/vector.h"
namespace Engine::Property {
using namespace Engine::Math;
class Transform : public IProperty {
  // If I just declare every member as mutable then whats even the point of the
  // const qualifiers as of the time of writing this, i have not the energy to
  // think of an answer.
private:
  mutable mat4x4f transform;
  vec3f getGlobalTranslation() const;

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
  const mat4x4f *getTransformMatrix() const;
};
} // namespace Engine::Property
