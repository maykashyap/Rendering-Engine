#pragma once

#include "vector.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <stdexcept>

namespace Engine::Math {

template <typename T, std::size_t Rows, std::size_t Columns> class Matrix {
public:
  static constexpr std::size_t ROWS = Rows;
  static constexpr std::size_t COLUMNS = Columns;
  static constexpr std::size_t SIZE = Rows * Columns;

private:
  std::array<T, SIZE> matrix_data;

public:
  // ── Constructors ──────────────────────────────────────────────────────

  // Default constructor always creates Identity matrix, which i think makes
  // sense
  Matrix() {
    matrix_data.fill(T(0));
    std::size_t identity_limit = std::min(Rows, Columns);
    for (std::size_t i = 0; i < identity_limit; ++i) {
      (*this)(i, i) = T(1); // Set diagonal to 1
    }
  }

  // You can use this to nullify the Identity matrix created above.
  explicit Matrix(T d) { matrix_data.fill(d); }

  // Flat row-major initializer list.
  // Achtung: this is a one-dimensional array internally.
  Matrix(std::initializer_list<T> list) {
    if (list.size() > SIZE)
      throw std::out_of_range("Matrix: initializer_list exceeds matrix size");
    matrix_data.fill(T(0));
    std::copy_n(list.begin(), std::min(list.size(), SIZE), matrix_data.begin());
  }

  // ── Assignment ────────────────────────────────────────────────────────

  Matrix &operator=(std::initializer_list<T> list) {
    if (list.size() > SIZE)
      throw std::out_of_range("Matrix: initializer_list exceeds matrix size");
    matrix_data.fill(T(0));
    std::copy_n(list.begin(), std::min(list.size(), SIZE), matrix_data.begin());
    return *this;
  }

  Matrix &operator=(T d) {
    matrix_data.fill(d);
    return *this;
  }

  // Assign a smaller matrix into this one — remaining elements zeroed.
  // Previosly I had an = overloading, but there is no point in assigning an
  // initialized matrix.
  template <std::size_t R, std::size_t C>
    requires(R <= Rows && C <= Columns)
  Matrix(const Matrix<T, R, C> &mat)
      : Matrix() { // Chains the identity constructor first!
    for (std::size_t r = 0; r < R; ++r) {
      for (std::size_t c = 0; c < C; ++c) {
        (*this)(r, c) = mat(r, c); // Overwrites top-left block
      }
    }
  }

  // ── Element access ────────────────────────────────────────────────────

  // 2D access — row, column (zero-indexed)
  T &operator()(std::size_t row, std::size_t col) {
    return matrix_data[row * COLUMNS + col];
  }
  const T &operator()(std::size_t row, std::size_t col) const {
    return matrix_data[row * COLUMNS + col];
  }

  // 1D flat access — useful for iteration, less clear for positional access
  T &operator[](std::size_t index) { return matrix_data[index]; }
  const T &operator[](std::size_t index) const { return matrix_data[index]; }

  T *c_array() { return matrix_data.data(); }
  const T *c_array() const { return matrix_data.data(); }

  // ── Matrix arithmetic ─────────────────────────────────────────────────

  Matrix &operator+=(const Matrix &rhs) {
    std::transform(matrix_data.begin(), matrix_data.end(),
                   rhs.matrix_data.begin(), matrix_data.begin(),
                   std::plus<T>());
    return *this;
  }

  Matrix &operator*=(T scalar) {
    std::transform(matrix_data.begin(), matrix_data.end(), matrix_data.begin(),
                   [scalar](T val) { return val * scalar; });
    return *this;
  }

  // Square matrices only — chaining rectangular multiplications
  // would change the type at each step which is confusing and rarely useful.
  Matrix &operator*=(const Matrix &rhs)
    requires(Rows == Columns)
  {
    *this = *this * rhs;
    return *this;
  }

  // General matrix multiplication — this(Rows x Columns) * other(Columns x C2)
  template <std::size_t C2>
  Matrix<T, Rows, C2> operator*(const Matrix<T, Columns, C2> &other) const {
    Matrix<T, Rows, C2> result;
    for (std::size_t row = 0; row < Rows; ++row)
      for (std::size_t col = 0; col < C2; ++col) {
        T sum = T(0);
        for (std::size_t k = 0; k < Columns; ++k)
          sum += (*this)(row, k) * other(k, col);
        result(row, col) = sum;
      }
    return result;
  }

  // ── 4x4 specific ─────────────────────────────────────────────────────

  // Translate by adding xyz to the rightmost column (column 3, 0-indexed).
  // In row-major homogeneous form: column 3 holds the translation.
  Matrix &operator+=(const Vec4<T> &translation)
    requires(Rows == 4 && Columns == 4)
  {
    (*this)(0, 3) += translation.x;
    (*this)(1, 3) += translation.y;
    (*this)(2, 3) += translation.z;
    return *this;
  }

  // Set homogeneous w scale — bottom-right element (3,3)
  Matrix &setW(T scalar)
    requires(Rows == 4 && Columns == 4)
  {
    (*this)(3, 3) = scalar;
    return *this;
  }

  Matrix &setTranslation(T x, T y, T z)
    requires(Rows == 4 && Columns == 4)
  {
    (*this)(0, 3) = x;
    (*this)(1, 3) = y;
    (*this)(2, 3) = z;
    return *this;
  }

  Matrix &setTranslation(const Vec4<T> &v)
    requires(Rows == 4 && Columns == 4)
  {
    return setTranslation(v.x, v.y, v.z);
  }

  // Mat4 * Vec4 — full homogeneous transform including w.
  // Row-major layout: row i occupies data[i*4 .. i*4+3].
  // Named transform() rather than operator* to keep call sites explicit:
  // "mat.transform(vec)" is unambiguous, "mat * vec" could be confused
  // with scalar multiply.
  // I mean this could be an operator overload, its not like the compiler wont
  // throw hands if I do something stupid.
  Vec4<T> transform(const Vec4<T> &v) const
    requires(Rows == 4 && Columns == 4)
  {
    return {
        (*this)(0, 0) * v.x + (*this)(0, 1) * v.y + (*this)(0, 2) * v.z +
            (*this)(0, 3) * v.w,
        (*this)(1, 0) * v.x + (*this)(1, 1) * v.y + (*this)(1, 2) * v.z +
            (*this)(1, 3) * v.w,
        (*this)(2, 0) * v.x + (*this)(2, 1) * v.y + (*this)(2, 2) * v.z +
            (*this)(2, 3) * v.w,
        (*this)(3, 0) * v.x + (*this)(3, 1) * v.y + (*this)(3, 2) * v.z +
            (*this)(3, 3) * v.w,
    };
  }

  // ── Static factories ──────────────────────────────────────────────────

  static Matrix Identity()
    requires(Rows == Columns)
  {
    Matrix result;
    for (std::size_t i = 0; i < Rows; ++i)
      result(i, i) = T(1);
    return result;
  }
  template <typename VecType, std::size_t Length>
  static Matrix ScalingMatrix(const VecBase<VecType, T, Length> &vector)
    requires(Rows == Columns)
  {
    static_assert(Length <= Rows,
                  "Vector length must not exceed matrix dimension.");
    Matrix result;
    for (std::size_t i = 0; i < Length; ++i)
      result(i, i) = vector[i];
    return result;
  }

  // ── Unimplemented — add as needed ─────────────────────────────────────
  T determinant() const;
  Matrix transpose() const;
  Matrix adjoint() const;
  Matrix inverse() const;
};

// ── Free functions ─────────────────────────────────────────────────────────
template <typename T, std::size_t Rows, std::size_t Columns, typename Derived>
inline VecN<T, Rows> operator*(const Matrix<T, Rows, Columns> &mat,
                               const VecBase<Derived, T, Columns> &vec) {
  VecN<T, Rows> result(T(0));
  for (std::size_t r = 0; r < Rows; ++r)
    for (std::size_t c = 0; c < Columns; ++c)
      result[r] += mat(r, c) * vec[c];
  return result;
}

template <typename T, std::size_t Rows, std::size_t Columns>
inline Matrix<T, Rows, Columns> operator+(Matrix<T, Rows, Columns> lhs,
                                          const Matrix<T, Rows, Columns> &rhs) {
  lhs += rhs;
  return lhs;
}

template <typename T, std::size_t Rows, std::size_t Columns>
inline Matrix<T, Rows, Columns> operator*(Matrix<T, Rows, Columns> lhs,
                                          T scalar) {
  lhs *= scalar;
  return lhs;
}

// ── Type aliases ───────────────────────────────────────────────────────────
template <typename T> using Matrix3x3 = Matrix<T, 3, 3>;
template <typename T> using Matrix4x4 = Matrix<T, 4, 4>;

using mat3x3f = Matrix3x3<float>;
using mat3x3d = Matrix3x3<double>;
using mat4x4f = Matrix4x4<float>;
using mat4x4d = Matrix4x4<double>;

} // namespace Engine::Math
