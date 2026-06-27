#pragma once

#include "IProperty.h"
#include "engine/lib/math/matrix.h"
#include "engine/lib/math/vector.h"
namespace Engine::Property {
using namespace Engine::Math;
class Transform : public IProperty {
  // If I just declare every member as mutable then whats even the point of the
  // const qualifiers as of the time of writing this, i have not the energy to
  // think of an answer.
private:
  mutable mat4x4f transform;

  // The Transform is set dirty at the start of render loop. When you first
  // query it it needs to be recalculated but subsequent queryings within the
  // same frame do not require the recalculation. Reduces O(N) significantly in
  // hierarchies. I could simplify this further by only marking dirty when
  // transform dependecies change but I dont have setter for them so it would be
  // awkward.
  mutable bool m_dirty = true;

public:
  vec3f translation;
  vec3f scale;
  vec3f rotation;
  Transform(const vec3f &translate = (vec3f)0, const vec3f &scale = (vec3f)1,
            const vec3f &rotate = (vec3f)0)
      : translation(translate), scale(scale), rotation(rotate) {
    ID = "Transform";
  }
  /**
   * @brief apply T * S * R into a matrix and return. Parent conscious.
   *
   * @return the Transform as mat4x4
   */
  const mat4x4f *getTransformMatrix() const;
  /**
   * @brief Transform member is parent relative if one exists. This function
   * returns the raw world coordinate of that transform. Traverses the
   * heierarchy to get it.
   *
   * @return vector 3 as translation
   */
  vec3f getGlobalTranslation() const;
  void markDirty() const { m_dirty = true; }
  // I just need this in the camera property. Also currently transform is only
  // marked dirty explicitly at frame which serves us well enough for recursive
  // getGlobalTranslation calculations each frames, but does not prevent
  // unecesasry recalculations between frames.
  bool isDirty() const { return m_dirty; }
};
} // namespace Engine::Property
