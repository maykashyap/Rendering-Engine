#pragma once

#include <string_view>
namespace Engine {
class IShader {
public:
  virtual ~IShader() = default;

  virtual void use() const = 0;
  virtual void setBool(std::string_view name, bool val) const = 0;
  virtual void setInt(std::string_view name, int val) const = 0;
  virtual void setFloat(std::string_view name, float val) const = 0;

  IShader(const IShader &) = delete;
  IShader &operator=(const IShader &) = delete;

protected:
  IShader() = default;
};
} // namespace Engine
