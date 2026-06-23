#include "engine/properties/Transform.h"
#include "engine/core/Entity.h"
#include "engine/lib/math3d.h"
#include "engine/lib/matrix.h"
#include "engine/lib/vector.h"

using namespace Engine::Property;
using namespace Engine::Math;

const mat4x4f *Transform::getTransformMatrix() const {
  vec3f localTranslation = getGlobalTranslation();
  mat3x3f rotationMatrix =
      Math::rotationMatrixEuler(rotation.x, rotation.y, rotation.z);
  mat3x3f scaleMatrix({scale.x, 0, 0, 0, scale.y, 0, 0, 0, scale.z});
  transform = rotationMatrix * scaleMatrix;
  transform.setTranslation(localTranslation);
  return &transform;
}

vec3f Transform::getGlobalTranslation() const {
  vec3f localTranslation = translation;
  if (m_owner->getParent()) {
    auto ownerTransform = m_owner->getParent()->getTransform();
    localTranslation =
        ownerTransform.getGlobalTranslation() +
        rotationMatrixEuler(ownerTransform.rotation) *
            (mat3x3f::ScalingMatrix(ownerTransform.scale) * translation);
  }
  return localTranslation;
}
