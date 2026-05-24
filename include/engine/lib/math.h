#ifndef MATH_H
#define MATH_H

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <initializer_list>

// SIMD - explore this i dont know what that is
namespace Engine::Math {

constexpr double EPSILON = 1e-9;
constexpr double PI = std::numbers::pi;

// Struct?
// Better memory allocation, not exactly SIMD but a step in the right direction
// H for homogenous.
template <typename T> struct alignas(16) VectorH {
  // meatballs
  union {
    struct {
      T x, y, z, w;
    };
    T data[4];
  };
  VectorH(T x = 0, T y = 0, T z = 0, T w = 0) : x(x), y(y), z(z), w(w) {}
  // Allows for initializer_list assignment.
  VectorH(std::initializer_list<T> vec_t) {
    auto it = vec_t.begin();
    x = *it++;
    y = *it++;
    z = *it++;
    w = *it++;
  }
  // The explicit keyword makes sure a random double doesnt get typecast to
  // vector where it wasnt supposed to
  // I also wonder about that w assignment. This could cause confusion.
  explicit VectorH(T d) : x(d), y(d), z(d), w(1) {}

  VectorH &operator=(const double data[4]) {
    this->x = data[0];
    this->y = data[1];
    this->z = data[2];
    this->w = data[3];
    return *this;
  }
  VectorH &operator=(double value) {
    // This will mostly be used for nullifying the vector anyway.
    x = y = z = w = value;
    return *this;
  }

  // eat the meatballs.
  double &operator[](std::size_t i) { return data[i]; }
  // A const accessor is neccessary for const contexts, like function calls and
  // rh values.
  const double &operator[](std::size_t i) const { return data[i]; }

  // += overlaoding and then overloading + to call += allows for expression such
  // as vec_a = vec_x + vec_y + vec_z the temp variables created in this
  // expression for vec_y + vec_z is rhv (const and pointer) and thus += should
  // also return a rhv (pointer) Dunno its all in my head so forgive me for the
  // shoddy explaination
  VectorH &operator+=(const VectorH &adendum) {
    this->x += adendum.x;
    this->y += adendum.y;
    this->z += adendum.z;
    return *this;
  }
  // same thing but allows for vec_a = vec_x + vec_y + d
  // I think.
  VectorH &operator+=(double d) {
    this->x += d;
    this->y += d;
    this->z += d;
    return *this;
  }

  double *c_array() { return data; };

  VectorH &normalize() {
    double mag = magnitude();
    // Edge case handeling where mag may be near zero.
    if (mag > EPSILON) {
      this->x /= mag;
      this->y /= mag;
      this->z /= mag;
    }
    return *this;
  }
  VectorH normalized() const {
    VectorH out = *this;
    return out.normalize();
  }

  double magnitude() const { return std::sqrt(x * x + y * y + z * z); }
};
// Inline prevents the program from jumping around by just copy pasting this
// code where it is needed. Performant.
template <typename T>
inline VectorH<T> operator+(VectorH<T> lhs, const VectorH<T> &rhs) {
  lhs += rhs;
  return lhs;
}
template <typename T>
inline VectorH<T> operator+(VectorH<T> lhs, const T scalar) {
  lhs.x += scalar;
  lhs.y += scalar;
  lhs.z += scalar;
  return lhs;
}
template <typename T>
inline VectorH<T> operator*(VectorH<T> lhs, const T scalar) {
  lhs.x *= scalar;
  lhs.y *= scalar;
  lhs.z *= scalar;
  return lhs;
}

template <typename T>
inline double vectorDot(const VectorH<T> &vec_a, const VectorH<T> &vec_b) {
  return vec_a.x * vec_b.x + vec_a.y * vec_b.y + vec_a.z * vec_b.z;
}
template <typename T>
inline VectorH<T> vectorCross(const VectorH<T> &vec_a,
                              const VectorH<T> &vec_b) {
  return VectorH<T>({vec_a.y * vec_b.z - vec_a.z * vec_b.y,
                     vec_a.z * vec_b.x - vec_a.x * vec_b.z,
                     vec_a.x * vec_b.y - vec_a.y * vec_b.x});
}

//_____________________________________________________________________________________

template <typename T, std::size_t Rows, std::size_t Columns> class Matrix {
public:
  static constexpr std::size_t ROWS = Rows;
  static constexpr std::size_t COLUMNS = Columns;
  static constexpr std::size_t SIZE = Rows * Columns;

private:
  std::array<T, SIZE> matrix_data;

public:
  Matrix() { matrix_data.fill(0.0); }
  explicit Matrix(T d) { matrix_data.fill(d); }
  // Allows for {...} initialization.
  // Achtung!! das ist aber ein eindimensionales array.
  Matrix(std::initializer_list<T> mat_t) {
    std::copy_n(mat_t.begin(), matrix_data.size(), matrix_data.begin());
  }

  // Same thing as the constructor.
  Matrix &operator=(std::initializer_list<T> mat_t) {
    std::copy_n(mat_t.begin(), matrix_data.size(), matrix_data.begin());
    return *this;
  }
  Matrix &operator=(T d) {
    matrix_data.fill(d);
    return *this;
  }

  // Allows for assigning a smaller matrix to a larger one.
  // the extra parts of the larger matrix will be filled by 0.
  template <std::size_t R, std::size_t C>
  typename std::enable_if<(R <= Rows && C <= Columns), Matrix &>::type
  operator=(const Matrix<T, R, C> &mat) {
    matrix_data.fill(T(0));
    for (std::size_t r = 0; r < R; r++) {
      for (std::size_t c = 0; c < C; c++) {
        (*this)(r, c) = mat(r, c);
      }
    }
    return *this;
  }

  // Add translation component.
  Matrix &operator+=(const VectorH<T> &translation) {
    static_assert(Rows == 4 && Columns == 4, "Not a 4x4 matrix.");
    (*this)(1, 4) += translation.x;
    (*this)(2, 4) += translation.y;
    (*this)(3, 4) += translation.z;
    return *this;
  }
  // Always Mat + Vec;
  Matrix &operator+(const VectorH<T> &translation) {
    static_assert(Rows == 4 && Columns == 4, "Not a 4x4 matrix.");
    *this += translation;
    return *this;
  }

  // Set the homogenous coordinate scaling. (4,4)
  template <std::size_t R = Rows, std::size_t C = Columns>
  typename std::enable_if<R == 4 && C == 4, Matrix &>::type SetW(T scalar) {
    (*this)(3, 3) = scalar;
    return *this;
  }

  template <std::size_t R = Rows, std::size_t C = Columns>
  typename std::enable_if<R == 4 && C == 4, Matrix &>::type
  SetTranslation(T x, T y, T z) {
    (*this)(0, 3) = x;
    (*this)(1, 3) = y;
    (*this)(2, 3) = z;
    return *this;
  }

  template <std::size_t R = Rows, std::size_t C = Columns>
  typename std::enable_if<R == 4 && C == 4, Matrix &>::type
  SetTranslation(const VectorH<T> &translation) {
    return SetTranslation(translation.x, translation.y, translation.z);
  }

  // this exists here so that I am always aware that its a MAT x VEC product and
  // not the other way around. if the parameters could move around it would be
  // wierd.
  template <std::size_t R = Rows, std::size_t C = Columns>
  typename std::enable_if<R == 4 && C == 4, VectorH<T>>::type
  operator*(const VectorH<T> &vec) {
    VectorH<T> vec_out = 0;
    vec_out.x = matrix_data[0] * vec.x + matrix_data[1] * vec.y +
                matrix_data[3] * vec.z;
    vec_out.y = matrix_data[4] * vec.x + matrix_data[4] * vec.y +
                matrix_data[5] * vec.z;
    vec_out.z = matrix_data[6] * vec.x + matrix_data[7] * vec.y +
                matrix_data[8] * vec.z;
    return vec_out; // we are returning a copy.
  }

  T &operator()(std::size_t row, std::size_t column) {
    return matrix_data[row * COLUMNS + column];
  }
  const T &operator()(std::size_t row, std::size_t column) const {
    return matrix_data[row * COLUMNS + column];
  }
  // !! one dimensional coordinates, Useful for interating, but can be
  // conterintuitive for positional access.
  T &operator[](std::size_t index) { return matrix_data[index]; }
  // For const contexts, like in rhv.
  const T &operator[](std::size_t index) const { return matrix_data[index]; }

  template <std::size_t Columns_2>
  Matrix<T, Rows, Columns_2>
  operator*(Matrix<T, Columns, Columns_2> &other) const {
    Matrix<T, Rows, Columns_2> result;
    for (std::size_t row = 0; row < Rows; row++) {
      for (std::size_t column = 0; column < Columns_2; column++) {
        T sum = T(0);
        for (std::size_t k = 0; k < Columns; k++)
          sum += (*this)(row, k) * other(k, column);
        result(row, column) = sum;
      }
    }
  }

  // So I can chain MATRIX multiplications.
  Matrix &operator*=(const Matrix &prod) {
    static_assert(
        Rows == Columns,
        "Matrix must be square for the use of *=."); // chaining rect mat
                                                     // multiplications would
                                                     // lead to confusion, also
                                                     // uneccessary.
    *this = *this * prod;
    return *this;
  }

  Matrix &operator+=(const Matrix &adendum) {
    std::transform(matrix_data.begin(), matrix_data.end(),
                   adendum.matrix_data.begin(), matrix_data.begin(),
                   std::plus<T>());
    return *this;
  }

  Matrix &operator*=(T scalar) {
    std::transform(matrix_data.begin(), matrix_data.end(), matrix_data.begin(),
                   [scalar](T val) { return val * scalar; });
    return *this;
  }

  T *c_array() { return matrix_data.data(); };

  template <std::size_t R = Rows, std::size_t C = Columns>
  static typename std::enable_if<R == C, Matrix>::type Identity() {
    Matrix result;
    for (std::size_t i = 0; i < Rows; i++)
      result(i, i) = T(i);
    return result;
  }

  // I don't see an immediate need of these functions.
  // I will impelement them as needed.
  double determinant();
  Matrix transpose();
  Matrix adjoint();
  Matrix inverse();
};

template <typename T, std::size_t Rows, std::size_t Columns>
inline Matrix<T, Rows, Columns>
operator+(Matrix<T, Rows, Columns> &mat_a,
          const Matrix<T, Rows, Columns> &mat_b) {
  mat_a += mat_b;
  return mat_b;
}
template <typename T, std::size_t Rows, std::size_t Columns>
inline Matrix<T, Rows, Columns> operator*(Matrix<T, Rows, Columns> &mat_a,
                                          T scalar) {
  mat_a += scalar;
  return mat_a;
}
template <typename T, std::size_t Rows, std::size_t Columns>
inline Matrix<T, Rows, Columns> operator*(Matrix<T, Rows, Columns> lhs,
                                          const Matrix<T, Rows, Columns> &rhs) {
  lhs *= rhs;
  return lhs;
}

// _____________________________________________________________________________________

using vecf = VectorH<float>;
using vecd = VectorH<double>;
using veci = VectorH<int>;

template <typename T> using Matrix3x3 = Matrix<T, 3, 3>;

template <typename T> using Matrix4x4 = Matrix<T, 4, 4>;

using mat3x3f = Matrix3x3<float>;
using mat3x3d = Matrix3x3<double>;
using mat4x4f = Matrix4x4<float>;
using mat4x4d = Matrix4x4<double>;

} // namespace Engine::Math
#endif
