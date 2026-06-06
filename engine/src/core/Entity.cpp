#include "engine/core/Entity.h"
#include <iostream>
#include <string_view>

using namespace Engine;

void Entity::addProperty(std::string_view ID, IProperty *property) {
  if (m_propertyMap.find(ID.data()) != m_propertyMap.end()) {
    std::cerr << "Duplicate ID: " << ID << "on " << m_name;
    return;
  }
  m_propertyMap.emplace(property->getID(), t_Property(property));
  property->setOwner(this);
}

template <typename T> T *Entity::getProperty(std::string_view ID) const {
  auto it = m_propertyMap.find(ID.data());
  if (it != m_propertyMap.end()) {
    return dynamic_cast<T *>(it->second.get());
  }
  return nullptr;
}

void Entity::removeProperty(std::string_view ID) {
  auto it = m_propertyMap.find(ID.data());
  if (it != m_propertyMap.end()) {
    m_propertyMap.erase(it);
  }
}
