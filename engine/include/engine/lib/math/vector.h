#pragma once

#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <numbers>
#include <stdexcept>

namespace Engine::Math {

constexpr double EPSILON = 1e-9;
constexpr double PI = std::numbers::pi;

/**
 * @brief CRTP Base for all Vector Types.
 * Fixed length Vectors up to 4 dimensions have named members (eg. x, y for
 * Vec2). The challenge is to have a base class that defines common
 * functionality. I could have a data array in the base class, but the primary
 * storage method is not the array directly but with an abstraction of the union
 * and the anonymous struct. So you just provide the base class with a subclass
 * handle and now you have access to all its members and function to do with as
 * you please.
 *
 * @tparam Derived  Child Class
 * @tparam T        Data type
 * @tparam LEN      Length of the vector
 */
template <typename Derived, typename T, std::size_t LEN> struct VecBase {

  /**
   * @brief Access vector member by index (0-indexed).
   *
   * @param i index
   * @return By reference.
   */
  T &operator[](std::size_t i) { return static_cast<Derived *>(this)->data[i]; }
  /**
   * @brief const access vector member by index (0-indexed).
   *
   * @param i index
   * @return Const by reference.
   */
  const T &operator[](std::size_t i) const {
    return static_cast<const Derived *>(this)->data[i];
  }

  /**
   * @brief raw c-style array
   *
   * @return pointer to data array.
   */
  T *c_array() { return static_cast<Derived *>(this)->data; }
  /**
   * @brief const raw c-style array
   *
   * @return const pointer to data array.
   */
  const T *c_array() const { return static_cast<const Derived *>(this)->data; }

  Derived &operator=(T value) {
    for (std::size_t i = 0; i < LEN; ++i)
      (*this)[i] = value;
    return *static_cast<Derived *>(this);
  }

  bool operator==(const Derived &rhs) const {
    for (std::size_t i = 0; i < LEN; ++i)
      if ((*this)[i] != rhs[i])
        return false;
    return true;
  }
  bool operator!=(const Derived &rhs) const { return !(*this == rhs); }
  // Component-wise vector * vector — all LEN components including w.
  // Distinct from operator*=(T scalar) which only touches xyz on Vec4.
  /**
   * @brief Component-wise vector * vector
   * all LEN components including w.
   * Distinct from operator*=(T scalar) which only touches xyz on Vec4.
   *
   * @param rhs
   * @return reference of this
   */
  Derived &operator*=(const Derived &rhs) {
    for (std::size_t i = 0; i < LEN; ++i)
      (*this)[i] *= rhs[i];
    return *static_cast<Derived *>(this);
  }

  friend Derived operator*(Derived lhs, const Derived &rhs) {
    lhs *= rhs;
    return lhs;
  }

  friend Derived operator+(Derived lhs, const Derived &rhs) {
    lhs += rhs;
    return lhs;
  }
  friend Derived operator+(Derived lhs, T scalar) {
    lhs += scalar;
    return lhs;
  }
  friend Derived operator*(Derived lhs, T scalar) {
    lhs *= scalar;
    return lhs;
  }

protected:
  VecBase() = default;
};

template <typename T, std::size_t N> struct VecN : VecBase<VecN<T, N>, T, N> {
  T data[N]{};

  VecN() = default;
  explicit VecN(T scalar) {
    for (std::size_t i = 0; i < N; ++i)
      data[i] = scalar;
  }
  /**
   * @brief implicit cast from VecN to Vec
   *
   * @tparam Derived
   */
  template <typename Derived>
  operator Derived() const
    requires std::is_base_of_v<VecBase<Derived, T, N>, Derived>
  {
    Derived result;
    for (std::size_t i = 0; i < N; ++i)
      result[i] = data[i];
    return result;
  }

  VecN &operator+=(const VecN &rhs) {
    for (std::size_t i = 0; i < N; ++i)
      data[i] += rhs.data[i];
    return *this;
  }
  VecN &operator+=(T scalar) {
    for (std::size_t i = 0; i < N; ++i)
      data[i] += scalar;
    return *this;
  }
  VecN &operator*=(T scalar) {
    for (std::size_t i = 0; i < N; ++i)
      data[i] *= scalar;
    return *this;
  }
  VecN &operator*=(const VecN &rhs) {
    for (std::size_t i = 0; i < N; ++i)
      data[i] *= rhs.data[i];
    return *this;
  }
};

// ── Vec2 ───────────────────────────────────────────────────────────────────
template <typename T> struct Vec2 : VecBase<Vec2<T>, T, 2> {

  union {
    struct {
      T x, y;
    };
    T data[2];
  };

  Vec2() {
    x = 0;
    y = 0;
  }
  Vec2(T x, T y) : x(x), y(y) {}
  explicit Vec2(T scalar) : x(scalar), y(scalar) {}
  Vec2(const VecN<T, 2> &v) : x(v[0]), y(v[1]) {}

  Vec2 &operator+=(const Vec2 &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  Vec2 &operator+=(T scalar) {
    x += scalar;
    y += scalar;
    return *this;
  }
  Vec2 &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  T magnitude() const { return std::sqrt(x * x + y * y); }

  /**
   * @brief normalize vector and write to self.
   *
   * @return refrence to this
   */
  Vec2 &normalize() {
    T mag = magnitude();
    if (mag > static_cast<T>(EPSILON)) {
      x /= mag;
      y /= mag;
    }
    return *this;
  }
  /**
   * @brief Normalize vector to a copy
   *
   * @return a normalized version of the vector
   */
  Vec2 normalized() const {
    Vec2 out = *this;
    return out.normalize();
  }
  using VecBase<Vec2<T>, T, 2>::operator*=;
};

// ── Vec3 ───────────────────────────────────────────────────────────────────
template <typename T> struct Vec3 : VecBase<Vec3<T>, T, 3> {

  union {
    struct {
      T x, y, z;
    };
    T data[3];
  };

  Vec3() {
    x = 0;
    y = 0;
    z = 0;
  }
  Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
  explicit Vec3(T scalar) : x(scalar), y(scalar), z(scalar) {}
  Vec3(const Vec2<T> &v, T z = T(0)) : x(v.x), y(v.y), z(z) {}
  Vec3(const VecN<T, 3> &v) : x(v[0]), y(v[1]), z(v[2]) {}

  Vec3 &operator+=(const Vec3 &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  Vec3 &operator+=(T scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
  }
  using VecBase<Vec3<T>, T, 3>::operator*=;
  Vec3 &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  T magnitude() const { return std::sqrt(x * x + y * y + z * z); }

  Vec3 &normalize() {
    T mag = magnitude();
    if (mag > static_cast<T>(EPSILON)) {
      x /= mag;
      y /= mag;
      z /= mag;
    }
    return *this;
  }
  Vec3 normalized() const {
    Vec3 out = *this;
    return out.normalize();
  }
};

// ── Vec4 — 16-byte aligned homogeneous 4D vector ───────────────────────
// w = 1 → point,  w = 0 → direction
// Arithmetic excludes w — caller sets w to match coordinate semantics.
template <typename T> struct alignas(16) Vec4 : VecBase<Vec4<T>, T, 4> {

  union {
    struct {
      T x, y, z, w;
    };
    T data[4];
  };

  Vec4() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
  }
  Vec4(T x = T(0), T y = T(0), T z = T(0), T w = T(0))
      : x(x), y(y), z(z), w(w) {}

  Vec4(std::initializer_list<T> list) : x(T(0)), y(T(0)), z(T(0)), w(T(0)) {
    if (list.size() > 4)
      throw std::out_of_range("VectorH: initializer_list exceeds 4 elements");
    auto it = list.begin();
    if (it != list.end())
      x = *it++;
    if (it != list.end())
      y = *it++;
    if (it != list.end())
      z = *it++;
    if (it != list.end())
      w = *it++;
  }
  Vec4(const VecN<T, 4> &v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

  // explicit — prevents accidental scalar → VectorH implicit conversion.
  // w = 1: scalar construction is a point, not a direction.
  explicit Vec4(T d) : x(d), y(d), z(d), w(T(1)) {}

  // Promote Vec3 — caller specifies whether it's a point (w=1) or direction
  // (w=0)
  Vec4(const Vec3<T> &v, T w = T(1)) : x(v.x), y(v.y), z(v.z), w(w) {}

  Vec4 &operator=(const T arr[4]) {
    x = arr[0];
    y = arr[1];
    z = arr[2];
    w = arr[3];
    return *this;
  }

  // Arithmetic — xyz only, w preserved
  Vec4 &operator+=(const Vec4 &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  Vec4 &operator+=(T scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
  }
  using VecBase<Vec4<T>, T, 4>::operator*=;
  Vec4 &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  T magnitude() const { return std::sqrt(x * x + y * y + z * z); }

  Vec4 &normalize() {
    T mag = magnitude();
    if (mag > static_cast<T>(EPSILON)) {
      x /= mag;
      y /= mag;
      z /= mag;
    }
    return *this;
  }
  Vec4 normalized() const {
    Vec4 out = *this;
    return out.normalize();
  }
};

// ── Free functions ─────────────────────────────────────────────────────────

// dot — generic over all LEN components
template <typename Derived, typename T, std::size_t LEN>
inline T dot(const VecBase<Derived, T, LEN> &a,
             const VecBase<Derived, T, LEN> &b) {
  T result = T(0);
  for (std::size_t i = 0; i < LEN; ++i)
    result += a[i] * b[i];
  return result;
}

// dot — VectorH overload: xyz only, w excluded
// More specific overload — compiler prefers this over the generic when
// both arguments are VectorH
template <typename T> inline T dot(const Vec4<T> &a, const Vec4<T> &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

// cross — Vec3 only
template <typename T> inline Vec3<T> cross(const Vec3<T> &a, const Vec3<T> &b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

// cross — VectorH: xyz only, w = 0 (result is always a direction)
template <typename T> inline Vec4<T> cross(const Vec4<T> &a, const Vec4<T> &b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x,
          T(0)};
}

// ── Type aliases ───────────────────────────────────────────────────────────
using vec2f = Vec2<float>;
using vec2d = Vec2<double>;
using vec2i = Vec2<int>;

using vec3f = Vec3<float>;
using vec3d = Vec3<double>;
using vec3i = Vec3<int>;

using vecf = Vec4<float>;
using vecd = Vec4<double>;
using veci = Vec4<int>;

} // namespace Engine::Math
