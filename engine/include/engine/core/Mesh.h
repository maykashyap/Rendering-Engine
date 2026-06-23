#pragma once

// Now there are a lot of ways to optimize this but i will just implement the
// basic version i need for now

#include "engine/lib/vector.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <numbers>
#include <ostream>
#include <stdexcept>
#include <strings.h>
#include <vector>

namespace Engine::Assets {
// Example type, you can make your own
// struct VertexPosColTex {
//   Math::Vec3<float> position;
//   Math::Vec3<float> color;
// };
// then you do:
// VertexLayout layout = {
//  {ComponentTypes::POSITION, ComponentSize::Float3, offsetof(VertexPosColTex,
//  position)},
//  ... so on for each type in vertex
// }
// then construct the Mesh

enum class ComponentTypes { POSITION, COLOR, TEXCOORD, NORMAL };
enum class ComponentSize : uint32_t { Float = 1, Float2 = 2, Float3 = 3 };

struct Component {
  ComponentTypes component;
  ComponentSize format;
  uint32_t offset;
};

using VertexLayout = std::vector<Component>;

// Submeshes can also be a useful functionality
// will implement when needed

class Mesh {
private:
  std::vector<uint8_t> m_vertices;
  std::vector<uint32_t> m_indices;
  uint32_t m_vertexCount;
  uint32_t m_indexCount;
  size_t m_stride;

  VertexLayout m_vertexLayout;

public:
  Mesh() noexcept = default;
  ~Mesh() noexcept = default;

  // Disable explicit copying to prevent accidental multi-megabyte duplicates
  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  // Enable Move Semantics (Fast transfers for asset management/registry)
  Mesh(Mesh &&other) noexcept = default;
  Mesh &operator=(Mesh &&other) noexcept = default;

  template <typename T>
  Mesh(std::vector<T> &vertices, std::vector<uint32_t> &indices,
       const VertexLayout &vertexLayout, size_t stride) {
    // the size of the template struct.
    m_stride = stride;

    m_vertexCount =
        static_cast<uint32_t>((vertices.size() * sizeof(T)) / m_stride);
    m_indexCount = static_cast<uint32_t>(indices.size());

    m_indices = std::move(indices);
    m_vertexLayout = std::move(vertexLayout);

    m_vertices.resize(m_vertexCount * m_stride);
    if (m_vertexCount > 0)
      memcpy(m_vertices.data(), vertices.data(), m_vertexCount * m_stride);
    else
      throw std::runtime_error("Invalid Mesh declaration.");
  }

  [[nodiscard]] const Component &getComponent(ComponentTypes componentType) {
    for (const auto &component : m_vertexLayout) {
      if (component.component == componentType)
        return component;
    }
    throw std::runtime_error("Vertex Component not found in layout.");
  }
  // Returns the raw byte blob ready for `glBufferData` or `vkCmdCopyBuffer`
  [[nodiscard]] const std::vector<uint8_t> &get_vertex_bytes() const noexcept {
    return m_vertices;
  }
  [[nodiscard]] const std::vector<uint32_t> &get_indices() const noexcept {
    return m_indices;
  }
  [[nodiscard]] uint32_t getVertexCount() const noexcept {
    return m_vertexCount;
  }
  [[nodiscard]] uint32_t getIndexCount() const noexcept { return m_indexCount; }
  [[nodiscard]] uint32_t getVertexSize() const noexcept { return m_stride; }

  [[nodiscard]] const VertexLayout &getLayout() const noexcept {
    return m_vertexLayout;
  }
  [[nodiscard]] bool isEmpty() const noexcept {
    // A mesh is empty if it has no vertices or no indices to draw
    return m_indices.empty() || m_vertices.empty();
  }

  void clearMesh() noexcept {
    m_vertices.clear();
    m_indices.clear();
    m_vertexLayout.clear();
    m_vertexCount = 0;
    m_indexCount = 0;
    m_stride = 0;
  }
};

class MeshGenerator {
public:
  template <int N> static Assets::Mesh Polygon(float angleOffset) {
    static_assert(N >= 3, "A polygon must have 3 or more sides");
    struct VertexPos {
      Math::vec2f Pos;
    };
    Assets::VertexLayout layout = {{Assets::ComponentTypes::POSITION,
                                    Assets::ComponentSize::Float2,
                                    offsetof(VertexPos, Pos)}};
    std::vector<float> vertices = {0.0f, 0.0f};
    std::vector<uint32_t> indices;

    constexpr float angle = (2.0f * std::numbers::pi) / static_cast<float>(N);
    for (int i = 1; i <= N; i++) {
      vertices.push_back(cos(i * angle + angleOffset));
      vertices.push_back(sin(i * angle + angleOffset));
      indices.push_back(0);
      indices.push_back(i);
      indices.push_back(i + 1 > N ? 1 : i + 1);
    }

    Mesh mesh(vertices, indices, layout, sizeof(VertexPos));
    return mesh;
  }
  static Assets::Mesh Square() {
    struct VertexPos {
      Math::vec2f Pos;
    };
    Assets::VertexLayout layout = {{Assets::ComponentTypes::POSITION,
                                    Assets::ComponentSize::Float2,
                                    offsetof(VertexPos, Pos)}};
    std::vector<float> vertices = {1.0f,  1.0f,  -1.0f, 1.0f,
                                   -1.0f, -1.0f, 1.0f,  -1.0f};
    std::vector<uint32_t> indices = {0, 1, 2, 3, 0, 2};
    return Mesh(vertices, indices, layout, sizeof(VertexPos));
  }
};
} // namespace Engine::Assets
