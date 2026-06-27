#include "engine/properties/CameraProperty.h"
#include "engine/core/Entity.h"
#include "engine/lib/math/math3d.h"
#include "engine/lib/math/matrix.h"
#include "engine/lib/math/vector.h"
#include <iostream>

using namespace Engine;
using namespace Engine::Math;

/**
 * @brief [TODO:description]
 *
 * @return [TODO:return]
 */
void Property::CameraProperty::setView() {
  // this is problematic
  // also what if transforms are not dirty but the view has not been assigned
  // yet.
  // if (!(m_lookAtTarget && m_lookAtTarget->getTransform().isDirty()) ||
  //     !m_owner->getTransform().isDirty())
  //   return;
  vec3f cameraTransform = m_owner->getTransform().getGlobalTranslation();
  vec3f i, j, k;
  if (!m_lookAtTarget) {
    mat3x3f uninvertedView =
        rotationMatrixEuler(m_owner->getTransform().rotation);
    i = vec3f(uninvertedView(0, 0), uninvertedView(1, 0), uninvertedView(2, 0));
    j = vec3f(uninvertedView(0, 1), uninvertedView(1, 1), uninvertedView(2, 1));
    k = vec3f(uninvertedView(0, 2), uninvertedView(1, 2), uninvertedView(2, 2));
  } else {
    k = m_lookAtTarget->getTransform().getGlobalTranslation() * 1.0f +
        m_owner->getTransform().getGlobalTranslation();
    k.normalize();

    j = rotationMatrixEuler(0.0f, m_owner->getTransform().rotation.y, 0.0f) *
        vec3f(0.0f, 1.0f, 0.0f);
    j.normalize();

    i = cross(j, k).normalize();
  }
  mat3x3f invertedRotation =
      mat3x3f({i.x, i.y, i.z, j.x, j.y, j.z, k.x, k.y, k.z});
  vec3f invertedTranslation =
      vec3f(dot(cameraTransform, i), dot(cameraTransform, j),
            dot(cameraTransform, k)) *
      -1.0f;
  m_viewMatrix = mat4x4f(invertedRotation).setTranslation(invertedTranslation);
  m_viewMatrix.setW(1.0f);
}
