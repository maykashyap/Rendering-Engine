// While I find this approach very interesting and Its definitely somthing to
// keep in mind, such complications are not worth it just use standard structs.
#pragma once

#include "engine/lib/vector.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>

namespace Engine::Assets {

using namespace Engine::Math;

// ── Tags ───────────────────────────────────────────────────────────────────
namespace Attrib {
struct Position {};
struct Normal {};
struct TexCoord {};
struct Color {};
struct Tangent {};
struct Bitangent {};
} // namespace Attrib

// ── Tag → Type binding (the key-value pair) ────────────────────────────────
template <typename Tag, typename Component> struct Slot {
  using tag_type = Tag;
  using comp_type = Component;
  Component data{};
};

// ── component_floats trait ─────────────────────────────────────────────────
template <typename T> struct component_floats;
template <typename T>
struct component_floats<Vec2<T>> : std::integral_constant<std::size_t, 2> {};
template <typename T>
struct component_floats<Vec3<T>> : std::integral_constant<std::size_t, 3> {};
template <typename T>
struct component_floats<Vec4<T>> : std::integral_constant<std::size_t, 4> {};

template <typename T>
inline constexpr std::size_t component_floats_v = component_floats<T>::value;

// ── Compile-time pack utilities ────────────────────────────────────────────
template <typename SearchTag, typename... Slots>
constexpr std::size_t index_of_tag() {
  constexpr bool matches[] = {
      std::is_same_v<SearchTag, typename Slots::tag_type>...};
  for (std::size_t i = 0; i < sizeof...(Slots); ++i) {
    if (matches[i])
      return i;
  }
  return sizeof...(Slots);
}

// Left Fold expression, the expression to the left is evaluated with every
// member of the "Pack" using the specified operator in this case ||
template <typename SearchTag, typename... Slots> constexpr bool has_tag() {
  return (std::is_same_v<SearchTag, typename Slots::tag_type> || ...);
}

// ── Vertex ─────────────────────────────────────────────────────────────────
template <typename... Slots> struct Vertex {

  static constexpr std::size_t FLOAT_COUNT =
      (component_floats_v<typename Slots::comp_type> + ...);

  template <typename Tag> static constexpr bool has = has_tag<Tag, Slots...>();

  std::tuple<Slots...> attributes;

  // ── Construction ───────────────────────────────────────────────────────
  explicit Vertex(const typename Slots::comp_type &...values)
      : attributes(Slots{values}...) {}

  Vertex() = default;

  // ── Named access ───────────────────────────────────────────────────────
  template <typename Tag> auto &get() {
    static_assert(has<Tag>,
                  "Vertex::get — tag not present in this vertex format.");
    return std::get<index_of_tag<Tag, Slots...>()>(attributes).data;
  }

  template <typename Tag> const auto &get() const {
    static_assert(has<Tag>,
                  "Vertex::get — tag not present in this vertex format.");
    return std::get<index_of_tag<Tag, Slots...>()>(attributes).data;
  }

  // ── Named set ──────────────────────────────────────────────────────────
  // declval = access members to the type without constructing.
  // decltype = get the type of an expression.
  // std::decay_t = removes all const qualifiers or references from the typename
  template <typename Tag>
  Vertex &
  set(const std::decay_t<decltype(std::declval<Vertex>().template get<Tag>())>
          &value) {
    static_assert(has<Tag>,
                  "Vertex::set — tag not present in this vertex format.");
    get<Tag>() = value;
    return *this;
  }

  // ── GPU flatten ────────────────────────────────────────────────────────
  std::array<float, FLOAT_COUNT> flatten() const {
    std::array<float, FLOAT_COUNT> result{};
    std::size_t offset = 0;

    // [&] captures all surrounding variables by reference
    // all the members of attributes as a variadic argument as cont reference
    // attributes will look like vec3, vec3, vec2
    std::apply(
        [&](const auto &...slot) {
          ((std::copy(slot.data.c_array(),
                      slot.data.c_array() +
                          component_floats_v<std::decay_t<decltype(slot.data)>>,
                      result.begin() + offset),
            offset += component_floats_v<std::decay_t<decltype(slot.data)>>),
           ...);
        },
        attributes);

    return result;
  }
};

// ── Predefined formats ─────────────────────────────────────────────────────
// Fixed: Added missing '<' to all templates below
using VertexPosOnly = Vertex<Slot<Attrib::Position, Vec3<float>>>;

using VertexPosNorm = Vertex<Slot<Attrib::Position, Vec3<float>>,
                             Slot<Attrib::Normal, Vec3<float>>>;

using VertexPosNormTex = Vertex<Slot<Attrib::Position, Vec3<float>>,
                                Slot<Attrib::Normal, Vec3<float>>,
                                Slot<Attrib::TexCoord, Vec2<float>>>;

using VertexPosNormTexTan = Vertex<
    Slot<Attrib::Position, Vec3<float>>, Slot<Attrib::Normal, Vec3<float>>,
    Slot<Attrib::TexCoord, Vec2<float>>, Slot<Attrib::Tangent, Vec3<float>>,
    Slot<Attrib::Bitangent, Vec3<float>>>;

using VertexPosColor = Vertex<Slot<Attrib::Position, Vec3<float>>,
                              Slot<Attrib::Color, Vec4<float>>>;

} // namespace Engine::Assets
