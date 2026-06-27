#pragma once

#include "engine/lib/math/matrix.h"
#include "engine/lib/math/vector.h"
#include <string_view>
namespace Engine::Assets {
class IShader {
public:
  virtual ~IShader() = default;

  virtual void use() const = 0;
  virtual void setUniform(std::string_view name, bool val) const = 0;
  virtual void setUniform(std::string_view name, int val) const = 0;
  virtual void setUniform(std::string_view name, float val) const = 0;
  virtual void setUniform(std::string_view name,
                          const Math::mat4x4f *matrix) const = 0;
  virtual void setUniform(std::string_view name,
                          const Math::vec3f *vec3) const = 0;

  IShader(const IShader &) = delete;
  IShader &operator=(const IShader &) = delete;

protected:
  IShader() = default;
};
} // namespace Engine::Assets
