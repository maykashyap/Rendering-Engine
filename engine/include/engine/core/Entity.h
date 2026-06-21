#pragma once

#include "IProperty.h"
#include "engine/properties/MeshProperty.h"
#include "engine/properties/ShaderProperty.h"
#include "engine/properties/Transform.h"

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

  Entity() = default;
  // Entity(std::string name) : m_name(std::move(name)) {}
  ~Entity() = default;

  Property::Transform &getTransform() { return m_transform; }
  const Property::Transform &getTransform() const { return m_transform; }

  Property::Mesh *getMesh() const { return m_mesh.get(); }
  Property::ShaderProgram *getShader() const { return m_shader.get(); }

  template <typename... Args> void setMesh(Args &&...args) {
    m_mesh = std::make_unique<Property::Mesh>(std::forward<Args>(args)...);
  }
  template <typename... Args> void setShader(Args &&...args) {
    m_shader =
        std::make_unique<Property::ShaderProgram>(std::forward<Args>(args)...);
  }
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
    // Fallback checks: If someone asks for "Transform", "Mesh", or "Shader"
    // generically, intercept it immediately to prevent map-searching
    // completely!
    if constexpr (std::is_same_v<T, Property::Transform>)
      return const_cast<Property::Transform *>(&m_transform);
    if constexpr (std::is_same_v<T, Property::Mesh>)
      return static_cast<T *>(m_mesh.get());
    if constexpr (std::is_same_v<T, Property::ShaderProgram>)
      return static_cast<T *>(m_shader.get());

    auto it = m_propertyMap.find(std::string(ID));
    if (it != m_propertyMap.end()) {
      return static_cast<T *>(it->second.get());
    }
    return nullptr;
  }

  void removeProperty(std::string_view ID) {
    auto it = m_propertyMap.find(std::string(ID));
    if (it != m_propertyMap.end()) {
      m_propertyMap.erase(it);
    }
  }

  // [[nodiscard]] const std::string &getName() const { return m_name; }

private:
  // This will make more sense when I have entity queues or something
  // std::string m_name = "";
  Property::Transform m_transform;
  std::unique_ptr<Property::Mesh> m_mesh = nullptr;
  std::unique_ptr<Property::ShaderProgram> m_shader = nullptr;

  std::unordered_map<std::string, t_Property> m_propertyMap;
};
} // namespace Engine
