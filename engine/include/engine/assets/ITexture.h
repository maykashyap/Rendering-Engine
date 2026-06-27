#pragma once
#include <cstdint>

namespace Engine::Assets {

class ITexture {
public:
  virtual ~ITexture() = default;

  virtual void bind(uint32_t slot = 0) const = 0;

  virtual uint32_t getWidth() const = 0;
  virtual uint32_t getHeight() const = 0;

  ITexture(const ITexture &) = delete;
  ITexture &operator=(const ITexture &) = delete;

protected:
  ITexture() = default;
};

} // namespace Engine::Assets
