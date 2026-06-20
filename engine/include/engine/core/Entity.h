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

  Entity() {}
  Entity(std::string name) : m_name(std::move(name)) {}
  ~Entity() { m_propertyMap.clear(); }

  // In the future I will implement a way to add property name on addition but
  // for now the best way is to just declare it in the Property class
  template <typename T, typename... Args> T &addProperty(Args &&...args) {
    static_assert(std::is_base_of_v<IProperty, T>,
                  "Properties must derive from Engine::IProperty.");

    auto property = std::make_unique<T>(std::forward<Args>(args)...);

    T &ref = *property;
    std::string ID = std::string(property->getID());
    m_propertyMap[ID] = std::move(property);
    return ref;
  }
  template <typename T> T *getProperty(std::string_view ID) const {
    auto it = m_propertyMap.find(std::string(ID));
    if (it != m_propertyMap.end()) {
      return dynamic_cast<T *>(it->second.get());
    }
    return nullptr;
  }

  void removeProperty(std::string_view ID) {
    auto it = m_propertyMap.find(std::string(ID));
    if (it != m_propertyMap.end()) {
      m_propertyMap.erase(it);
    }
  }

  [[nodiscard]] const std::string &getName() const { return m_name; }

private:
  std::string m_name = "";
  std::unordered_map<std::string, t_Property> m_propertyMap;
};
} // namespace Engine
