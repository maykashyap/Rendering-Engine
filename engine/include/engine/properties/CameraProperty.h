#pragma once

#include "engine/lib/math/matrix.h"
#include "engine/lib/math/vector.h"
#include "engine/properties/IProperty.h"

namespace Engine::Property {
class CameraProperty final : public IProperty {
public:
  CameraProperty() { ID = "Camera"; }
  const Math::mat4x4f &getViewMatrix() const { return m_viewMatrix; }
  void lookAt(const Entity &entity) { m_lookAtTarget = &entity; }
  const Math::mat4x4f &getProjectionMatrix() const {
    return m_projectionMatrix;
  }

  void setView();

  void setPerspective(/*args*/);
  void setOrthographic(float left, float right, float top, float bottom,
                       float near, float far) {
    m_projectionMatrix = Math::mat4x4f(
        {2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left), //
         0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom), //
         0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near),      //
         0.0f, 0.0f, 0.0f, 1.0f});
  }

private:
  Math::mat4x4f m_viewMatrix = Math::mat4x4f();
  Math::mat4x4f m_projectionMatrix = Math::mat4x4f();
  const Entity *m_lookAtTarget = nullptr;
};
} // namespace Engine::Property
