#pragma once

// Now there are a lot of ways to optimize this but i will just implement the
// basic version i need for now

#include <cstddef>
#include <cstdint>
#include <cstring>
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
       VertexLayout &vertexLayout) {
    m_stride = sizeof(T);
    m_vertexCount = static_cast<uint32_t>(vertices.size());
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
    throw std::runtime_error("Vertex Component not found, in layout.");
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
  void clearMesh() noexcept {
    m_vertices.clear();
    m_indices.clear();
    m_vertexLayout.clear();
    m_vertexCount = 0;
    m_indexCount = 0;
    m_stride = 0;
  }
};
} // namespace Engine::Assets
