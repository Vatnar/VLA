#pragma once
#include <iosfwd>
#include <print>
#include <string>

namespace VLA {

template<class T, size_t N>
struct Vector {
  T v[N];
  constexpr Vector operator+(const Vector& other) const {
    Vector result;
    for (std::size_t i{0}; i < N; i++) {
      result.v[i] = v[i] + other.v[i];
    }
    return result;
  }

  constexpr Vector operator-(const Vector& other) const {
    Vector result;
    for (std::size_t i{0}; i < N; i++) {
      result.v[i] = v[i] - other.v[i];
    }

    return result;
  }
  constexpr Vector operator*(const T& scalar) const {
    Vector result;
    for (std::size_t i{0}; i < N; i++) {
      result.v[i] = v[i] * scalar;
    }
    return result;
  }

  constexpr T& operator[](std::size_t idx) { return v[idx]; }
  constexpr const T& operator[](std::size_t idx) const { return v[idx]; }

  constexpr friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << std::string("[ ");
    for (int i = 0; i < N; i++) {
      os << v.v[i] << " ";
    }
    os << std::string(" ]");
    return os;
  }
};


template<class T, std::size_t N>
constexpr Vector<T, N> operator*(const T& scalar, const Vector<T, N>& v) {
  return v * scalar;
}

template<typename T>
using Vector4 = Vector<T, 4>;
template<typename T>
using Vector3 = Vector<T, 3>;

template<typename T>
using Vector2 = Vector<T, 2>;

extern template class Vector<float, 4>;
extern template class Vector<float, 3>;
extern template class Vector<float, 2>;

extern template class Vector<int, 4>;
extern template class Vector<int, 3>;
extern template class Vector<int, 2>;

using Vector4f = Vector4<float>;
using Vector3f = Vector3<float>;
using Vector2f = Vector2<float>;

using Vector4i = Vector4<int>;
using Vector3i = Vector3<int>;
using Vector2i = Vector2<int>;
} // namespace VLA

namespace std {

template<typename T, size_t N>
struct formatter<VLA::Vector<T, N>, char> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template<typename Ctx>
  auto format(const VLA::Vector<T, N>& v, Ctx& ctx) const {
    auto out = ctx.out();
    *out++ = '[';
    for (std::size_t i = 0; i < N; ++i) {
      out = format_to(out, "{}{}", v.v[i], (i + 1 < N ? ' ' : ']'));
    }
    return out;
  }
};
} // namespace std
