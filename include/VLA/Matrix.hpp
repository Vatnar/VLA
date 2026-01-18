#pragma once
#include <VLA/Vector.hpp>
#include <cstddef>
namespace VLA {

template<typename T, std::size_t N, std::size_t M>
struct Matrix {
  T a[N * M];


  // ranges
  using iterator = T*;
  using const_iterator = const T*;
  constexpr iterator begin() noexcept { return a; }
  // NOTE: end() should point to one past the last element
  constexpr iterator end() noexcept { return a + N * M; }
  constexpr const_iterator begin() const noexcept { return a; }
  // NOTE: end() should point to one past the last element
  constexpr const_iterator end() const noexcept { return a + N * M; }


  // operator overloads

  constexpr T& operator[](std::size_t idx) { return a[idx]; }
  constexpr const T& operator[](std::size_t idx) const { return a[idx]; }

  constexpr Vector<T, N> operator*(const Vector<T, M> v) {
    Vector<T, N> result{};

    const std::size_t stride = M;

    for (std::size_t col{0}; col < M; col++) {
      for (std::size_t row{0}; row < N; row++) {
        result[row] += v[row] * a[col + row * stride];
      }
    }

    return result;
  }
  constexpr Matrix operator*(const T& scalar) const {
    Matrix result;
    for (std::size_t i{0}; i < N; i++) {
      result.a[i] = a[i] * scalar;
    }
    return result;
  }


  // TODO: General determinant calculation for N x M matrix
  // STUDY: https://en.wikipedia.org/wiki/Leibniz_formula_for_determinants
  //  https://en.wikipedia.org/wiki/Determinant
};

template<class T, std::size_t N, std::size_t M>
constexpr Matrix<T, N, M> operator*(const T& scalar, const Matrix<T, N, M>& a) {
  return a * scalar;
}

extern template class Matrix<float, 4, 4>;
using Matrix4x4f = Matrix<float, 4, 4>;

} // namespace VLA
