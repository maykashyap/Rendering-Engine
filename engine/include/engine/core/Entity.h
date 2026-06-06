#pragma once

#include "IProperty.h"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
namespace Engine {
class IProperty; // forward declaration because headers are being included
                 // awkwardly
class Entity {
public:
  // the delete handler is default, and that shouldnt matter for me.
  using t_Property = std::unique_ptr<IProperty>;

  Entity(std::string name) : m_name(std::move(name)) {}
  ~Entity() { m_propertyMap.clear(); }

  void addProperty(std::string_view ID, IProperty *property);
  template <typename T> T *getProperty(std::string_view ID) const;
  void removeProperty(std::string_view ID);

  [[nodiscard]] const std::string &getName() const { return m_name; }

private:
  std::string m_name;
  std::unordered_map<std::string, t_Property> m_propertyMap;
};
} // namespace Engine
