#pragma once

#include "engine/core/IProperty.h"
#include "engine/lib/matrix.h"
#include "engine/lib/vector.h"
namespace Engine::Property {
class Transform : public IProperty {
public:
  Math::mat4x4f transform;
  Transform(Math::mat4x4f transformation)
      : transform(std::move(transformation)) {
    ID = "Transform";
    // Ill do the math later,
    // but not only does this get the 3 vectors
    // but it also calculates and assigns the transformed vector.
  }
};
} // namespace Engine::Property
