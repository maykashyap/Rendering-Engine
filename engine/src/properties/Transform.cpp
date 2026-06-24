#include "engine/properties/Transform.h"
#include "engine/core/Entity.h"
#include "engine/lib/math3d.h"
#include "engine/lib/matrix.h"
#include "engine/lib/vector.h"

using namespace Engine::Property;
using namespace Engine::Math;

const mat4x4f *Transform::getTransformMatrix() const {
  if (!m_dirty)
    return &transform;
  vec3f localTranslation = getGlobalTranslation();
  mat3x3f rotationMatrix = Math::rotationMatrixEuler(rotation);
  mat3x3f scaleMatrix = mat3x3f::ScalingMatrix(scale);
  transform = rotationMatrix * scaleMatrix;
  transform.setTranslation(localTranslation);
  m_dirty = false;
  return &transform;
}

vec3f Transform::getGlobalTranslation() const {
  auto localTranslation = Vec4<float>(translation);
  if (m_owner->getParent()) {
    auto ownerTransform =
        m_owner->getParent()->getTransform().getTransformMatrix();
    localTranslation = ownerTransform->transform(translation);
  }
  return vec3f(localTranslation.x, localTranslation.y, localTranslation.z);
}
