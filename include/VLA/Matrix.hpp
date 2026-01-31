#pragma once
#include <VLA/Vector.hpp>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <ranges>


// TODO: Change to use column-major instead to interact natively with Vulkan without transposing due
// to cache efficiency.
namespace VLA {

/**
 * Row-major matrix, can be transposed into column-major
 * @tparam T Primitive type
 * @tparam M Row count
 * @tparam N Column count
 */
template<typename T, std::size_t M, std::size_t N>
struct Matrix {
  // STUDY: STD::ARRAY
  std::array<T, M * N> A;


  // ranges
  using iterator = T*;
  using const_iterator = const T*;
  constexpr iterator begin() noexcept { return A.data(); }
  // NOTE: end() should point to one past the last element
  constexpr iterator end() noexcept { return A.data() + M * N; }
  [[nodiscard]] constexpr const_iterator begin() const noexcept { return A.data(); }
  // NOTE: end() should point to one past the last element
  [[nodiscard]] constexpr const_iterator end() const noexcept { return A.data() + M * N; }


  [[nodiscard]] constexpr Vector<T, N> Row(const std::size_t index) const {
    const auto rowCount = M;
    const auto colCount = N;
    assert(index < rowCount && "Row index specified cannot be higher than the amount of rows.");
    Vector<T, N> result{};


    for (std::size_t i{0}; i < colCount; i++) {
      result[i] = A[index * colCount + i];
    }
    return result;
  }

  [[nodiscard]] constexpr Vector<T, M> Column(const std::size_t index) const {
    const auto rowCount = M;
    const auto colCount = N;
    assert(index < colCount &&
           "Column index specified cannot be higher than the amount of columns");
    Vector<T, M> result{};

    for (std::size_t i{0}; i < rowCount; i++) {
      result[i] = A[index + i * colCount];
    }
    return result;
  }

  [[nodiscard]] constexpr Matrix<T, N, M> Transposed() const {
    Matrix<T, N, M> out{};

    for (std::size_t r = 0; r < M; ++r) {
      for (std::size_t c = 0; c < N; ++c) {
        out.A[c * M + r] = A[r * N + c];
      }
    }
    return out;
  }

  // NOTE: Was thinking about implementing determinants, however they are not really that useful.
  // Rather implement echelon operations for reducing matrices into identity matrix to find inverse.
  // Or to triangular matrices for determinants, which i presume might be faster since it doesnt
  // require recursion.

  [[nodiscard]] constexpr T Determinant() const {
    // TODO: row reduce until triangular.    //  then calculate determinant.

    // no need for this yet.
    return {};
  }

  // TODO: General determinant calculation for N x M matrix
  // STUDY: https://en.wikipedia.org/wiki/Leibniz_formula_for_determinants
  //  https://en.wikipedia.org/wiki/Determinant

  [[nodiscard]] constexpr std::array<T, M * N> ToRowMajor() const { return Transposed().A; }

  // operator overloads


  constexpr T& operator[](std::size_t idx) { return A[idx]; }
  constexpr const T& operator[](std::size_t idx) const { return A[idx]; }

  template<std::size_t R>
  constexpr Matrix<T, M, R> operator*(const Matrix<T, N, R> right) const {
    const auto rowCount = M;
    const auto colCount = R;


    Matrix<T, M, R> result;

    for (std::size_t i{0}; i < rowCount; i++) {
      for (std::size_t j{0}; j < colCount; j++) {
        result.A[i * colCount + j] = Vector<T, N>::Dot(Row(i), right.Column(j));
      }
    }
    return result;
  }


  constexpr Vector<T, M> operator*(const Vector<T, N> v) const {
    Vector<T, M> result{};

    const std::size_t stride = N;

    for (std::size_t col{0}; col < N; col++) {
      for (std::size_t row{0}; row < M; row++) {
        result[row] += v[row] * A[col + row * stride];
      }
    }

    return result;
  }

  constexpr Matrix operator*(const T& scalar) const {
    Matrix result{};
    std::ranges::copy(A | std::views::transform([scalar](T elem) { return elem * scalar; }),
                      std::data(result.A));
    return result;
  }


  constexpr friend std::ostream& operator<<(std::ostream& os, const Matrix& a) {
    const auto colCount = N;

    for (std::size_t i{0}; i < colCount; i++) {
      auto v = a.Row(i);
      os << v << '\n';
    }
    return os;
  }
};

template<class T, std::size_t M, std::size_t N>
constexpr Matrix<T, M, N> operator*(const T& scalar, const Matrix<T, M, N>& a) {
  return a * scalar;
}

extern template class Matrix<float, 4, 4>;
using Matrix4x4f = Matrix<float, 4, 4>;

} // namespace VLA


namespace std {
template<typename T, size_t M, size_t N>
struct formatter<VLA::Matrix<T, M, N>, char> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template<typename FormatContext>
  auto format(const VLA::Matrix<T, M, N>& mat, FormatContext& ctx) const {
    auto out = ctx.out();
    for (std::size_t i = 0; i < M; ++i) {
      out = std::format_to(out, "{}", mat.Row(i));
      if (i + 1 < M)
        *out++ = '\n';
    }
    return out;
  }
};
} // namespace std


constexpr VLA::Matrix4x4f RotationShear() {
  // clang-format off
  return VLA::Matrix4x4f{
    1.0, 3.5, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    -5.0, 3.0, 1.0, 3.0,
    2.0, 0.0, 0.0, 1.0
  };
  // clang-format on
}

// NOTE: TESTS
