#pragma once

#include "Entity.h"
#include <string>
#include <string_view>
namespace Engine {
class Entity;
class IProperty {
private:
protected:
  std::string ID; // Maybe later I can store it as a hash
  Entity *m_owner;

public:
  [[nodiscard]] std::string_view getID() const noexcept { return ID; };
  void setID(std::string_view ID) { this->ID = ID; }
  void setOwner(Entity *owner) { m_owner = owner; }
  virtual ~IProperty() = default;
};
} // namespace Engine
