#pragma once

#include "engine/core/IProperty.h"
#include "engine/lib/matrix.h"
#include "engine/lib/vector.h"
namespace Engine::Property {
class Transform : IProperty {
public:
  union {
    struct {
      Math::vec3f translation;
      Math::vec3f rotation;
      Math::vec3f scale;
    };
    Math::mat4x4f transform;
  };
  Transform(Math::mat4x4f transformation) {
    ID = "Transform";
    // Ill do the math later,
    // but not only does this get the 3 vectors
    // but it also calculates and assigns the transformed vector.
    this->translation = (Math::vec3f)0;
    this->rotation = (Math::vec3f)0;
    this->scale = (Math::vec3f)0;
  }
};
} // namespace Engine::Property
