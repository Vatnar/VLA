#include <VLA/Matrix.hpp>
#include <VLA/Vector.hpp>
#include <algorithm>
#include <array>
#include <numeric>

#include "doctest.h"

namespace {
template<typename T, std::size_t M, std::size_t N>
constexpr T at_rc(const VLA::Matrix<T, M, N>& A, std::size_t r, std::size_t c) {
  return A.A[r * N + c]; // row-major contract: idx = r*N + c
}
} // namespace

TEST_CASE("Matrix: storage and views") {
using VLA::Matrix;

SUBCASE("Row-major linearization sanity") {
  const Matrix<int, 2, 3> a{
      std::array<int, 6>{1, 2, 3, 4, 5, 6}
  };

  CHECK(a.A[0] == 1);
  CHECK(a.A[1] == 2);
  CHECK(a.A[2] == 3);
  CHECK(a.A[3] == 4);

  CHECK(at_rc(a, 0, 0) == 1);
  CHECK(at_rc(a, 0, 2) == 3);
  CHECK(at_rc(a, 1, 0) == 4);
  CHECK(at_rc(a, 1, 2) == 6);
}

SUBCASE("Row(i) returns expected row vector") {
  const Matrix<int, 2, 3> a{
      std::array<int, 6>{1, 2, 3, 4, 5, 6}
  };
  const auto r0 = a.Row(0);
  const auto r1 = a.Row(1);

  CHECK(r0[0] == 1);
  CHECK(r0[1] == 2);
  CHECK(r0[2] == 3);

  CHECK(r1[0] == 4);
  CHECK(r1[1] == 5);
  CHECK(r1[2] == 6);
}

SUBCASE("Column(j) returns expected column vector") {
  const Matrix<int, 2, 3> a{
      std::array<int, 6>{1, 2, 3, 4, 5, 6}
  };
  const auto c0 = a.Column(0);
  const auto c2 = a.Column(2);

  CHECK(c0[0] == 1);
  CHECK(c0[1] == 4);

  CHECK(c2[0] == 3);
  CHECK(c2[1] == 6);
}
} // TEST_CASE("Matrix: storage and views")

TEST_CASE("Matrix: transpose") {
using VLA::Matrix;

SUBCASE("2x2 transpose matches expected") {
  const Matrix<int, 2, 2> a{
      std::array<int, 4>{1, 2, 3, 4}
  };

  const auto at = a.Transposed();
  const Matrix<int, 2, 2> expected{
      std::array<int, 4>{1, 3, 2, 4}
  };

  CHECK(at.A == expected.A);
}

SUBCASE("2x3 transpose matches expected") {
  const Matrix<int, 2, 3> a{
      std::array<int, 6>{1, 2, 3, 4, 5, 6}
  };

  const auto at = a.Transposed();
  const Matrix<int, 3, 2> expected{
      std::array<int, 6>{1, 4, 2, 5, 3, 6}
  };

  CHECK(at.A == expected.A);
}

SUBCASE("Transpose definition: out(c,r) == in(r,c)") {
  const Matrix<int, 3, 4> a{
      std::array<int, 12>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
  };

  const auto at = a.Transposed(); // 4x3

  for (std::size_t r = 0; r < 3; ++r) {
    for (std::size_t c = 0; c < 4; ++c) {
      CHECK(at_rc(at, c, r) == at_rc(a, r, c));
    }
  }
}

SUBCASE("Double transpose returns original") {
  const Matrix<int, 4, 3> a{
      std::array<int, 12>{9, 1, 7, 3, 3, 2, 8, 0, 5, 6, 4, 4}
  };

  const auto a_tt = a.Transposed().Transposed();
  CHECK(a_tt.A == a.A);
}
} // TEST_CASE("Matrix: transpose")

TEST_CASE("Matrix: scalar multiply") {
using VLA::Matrix;

SUBCASE("Matrix * scalar") {
  const Matrix<int, 2, 3> a{
      std::array<int, 6>{1, 2, 3, 4, 5, 6}
  };
  const auto b = a * 2;

  CHECK(b.A == std::array<int, 6>{2, 4, 6, 8, 10, 12});
}

SUBCASE("scalar * Matrix") {
  const Matrix<int, 2, 2> a{
      std::array<int, 4>{1, 2, 3, 4}
  };
  const auto b = 3 * a;

  CHECK(b.A == std::array<int, 4>{3, 6, 9, 12});
}
} // TEST_CASE("Matrix: scalar multiply")

TEST_CASE("Matrix: matrix multiply") {
using VLA::Matrix;

SUBCASE("Identity acts as identity") {
  const Matrix<int, 2, 2> I{
      std::array<int, 4>{1, 0, 0, 1}
  };
  const Matrix<int, 2, 2> A{
      std::array<int, 4>{5, 6, 7, 8}
  };

  const auto left = I * A;
  const auto right = A * I;

  CHECK(left.A == A.A);
  CHECK(right.A == A.A);
}

SUBCASE("Known 2x3 * 3x2 product") {
  // A (2x3):
  // [1 2 3
  //  4 5 6]
  const Matrix<int, 2, 3> A{
      std::array<int, 6>{1, 2, 3, 4, 5, 6}
  };

  // B (3x2):
  // [7  8
  //  9  10
  // 11 12]
  const Matrix<int, 3, 2> B{
      std::array<int, 6>{7, 8, 9, 10, 11, 12}
  };

  // C = A*B (2x2):
  // [ 58  64
  //  139 154 ]
  const auto C = A * B;
  CHECK(C.A == std::array<int, 4>{58, 64, 139, 154});
}
} // TEST_CASE("Matrix: matrix multiply")

TEST_CASE("Matrix: matrix * vector") {
using VLA::Matrix;
using VLA::Vector;

SUBCASE("Identity leaves vector unchanged") {
  const Matrix<float, 4, 4> I{
      std::array<float, 16>{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}
  };

  const Vector<float, 4> v{1.f, 2.f, 3.f, 1.f};
  const auto out = I * v;

  CHECK(out[0] == doctest::Approx(1.f));
  CHECK(out[1] == doctest::Approx(2.f));
  CHECK(out[2] == doctest::Approx(3.f));
  CHECK(out[3] == doctest::Approx(1.f));
}

SUBCASE("Known 2x3 * vec3") {
  const Matrix<float, 2, 3> A{
      std::array<float, 6>{1, 2, 3, 4, 5, 6}
  };
  const Vector<float, 3> v{1.f, 1.f, 1.f};

  const auto out = A * v;
  CHECK(out[0] == doctest::Approx(6.f));  // 1+2+3
  CHECK(out[1] == doctest::Approx(15.f)); // 4+5+6
}
} // TEST_CASE("Matrix: matrix * vector")

TEST_CASE("Matrix: iterators cover storage") {
using VLA::Matrix;

SUBCASE("begin/end span all elements") {
  const Matrix<int, 2, 3> a{
      std::array<int, 6>{1, 2, 3, 4, 5, 6}
  };

  const auto count = static_cast<std::size_t>(std::distance(a.begin(), a.end()));
  CHECK(count == a.A.size());

  const int sum_it = std::accumulate(a.begin(), a.end(), 0);
  CHECK(sum_it == 21);
}
} // TEST_CASE("Matrix: iterators cover storage")
