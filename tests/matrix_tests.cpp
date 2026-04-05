#include <VLA/Matrix.hpp>
#include <VLA/Vector.hpp>
#include <gtest/gtest.h>
#include <array>
#include <numeric>

namespace {
template<typename T, std::size_t M, std::size_t N>
constexpr T at_rc(const VLA::Matrix<T, M, N>& A, std::size_t r, std::size_t c) {
  return A.A[r * N + c];
}
} // namespace

TEST(MatrixTest, StorageAndViews_RowMajorLinearization) {
  using VLA::Matrix;
  const Matrix<int, 2, 3> a{std::array<int, 6>{1, 2, 3, 4, 5, 6}};

  EXPECT_EQ(a.A[0], 1);
  EXPECT_EQ(a.A[1], 2);
  EXPECT_EQ(a.A[2], 3);
  EXPECT_EQ(a.A[3], 4);

  EXPECT_EQ(at_rc(a, 0, 0), 1);
  EXPECT_EQ(at_rc(a, 0, 2), 3);
  EXPECT_EQ(at_rc(a, 1, 0), 4);
  EXPECT_EQ(at_rc(a, 1, 2), 6);
}

TEST(MatrixTest, StorageAndViews_RowReturnsExpectedRowVector) {
  using VLA::Matrix;
  const Matrix<int, 2, 3> a{std::array<int, 6>{1, 2, 3, 4, 5, 6}};
  const auto r0 = a.Row(0);
  const auto r1 = a.Row(1);

  EXPECT_EQ(r0[0], 1);
  EXPECT_EQ(r0[1], 2);
  EXPECT_EQ(r0[2], 3);

  EXPECT_EQ(r1[0], 4);
  EXPECT_EQ(r1[1], 5);
  EXPECT_EQ(r1[2], 6);
}

TEST(MatrixTest, StorageAndViews_ColumnReturnsExpectedColumnVector) {
  using VLA::Matrix;
  const Matrix<int, 2, 3> a{std::array<int, 6>{1, 2, 3, 4, 5, 6}};
  const auto c0 = a.Column(0);
  const auto c2 = a.Column(2);

  EXPECT_EQ(c0[0], 1);
  EXPECT_EQ(c0[1], 4);

  EXPECT_EQ(c2[0], 3);
  EXPECT_EQ(c2[1], 6);
}

TEST(MatrixTest, Transpose_2x2MatchesExpected) {
  using VLA::Matrix;
  const Matrix<int, 2, 2> a{std::array<int, 4>{1, 2, 3, 4}};
  const auto at = a.Transposed();
  const Matrix<int, 2, 2> expected{std::array<int, 4>{1, 3, 2, 4}};

  EXPECT_EQ(at.A, expected.A);
}

TEST(MatrixTest, Transpose_2x3MatchesExpected) {
  using VLA::Matrix;
  const Matrix<int, 2, 3> a{std::array<int, 6>{1, 2, 3, 4, 5, 6}};
  const auto at = a.Transposed();
  const Matrix<int, 3, 2> expected{std::array<int, 6>{1, 4, 2, 5, 3, 6}};

  EXPECT_EQ(at.A, expected.A);
}

TEST(MatrixTest, Transpose_DefinitionOutCRequalsInRC) {
  using VLA::Matrix;
  const Matrix<int, 3, 4> a{std::array<int, 12>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};
  const auto at = a.Transposed();

  for (std::size_t r = 0; r < 3; ++r) {
    for (std::size_t c = 0; c < 4; ++c) {
      EXPECT_EQ(at_rc(at, c, r), at_rc(a, r, c));
    }
  }
}

TEST(MatrixTest, Transpose_DoubleTransposeReturnsOriginal) {
  using VLA::Matrix;
  const Matrix<int, 4, 3> a{std::array<int, 12>{9, 1, 7, 3, 3, 2, 8, 0, 5, 6, 4, 4}};
  const auto a_tt = a.Transposed().Transposed();

  EXPECT_EQ(a_tt.A, a.A);
}

TEST(MatrixTest, ScalarMultiply_MatrixTimesScalar) {
  using VLA::Matrix;
  const Matrix<int, 2, 3> a{std::array<int, 6>{1, 2, 3, 4, 5, 6}};
  const auto b = a * 2;

  EXPECT_EQ(b.A, (std::array<int, 6>{2, 4, 6, 8, 10, 12}));
}

TEST(MatrixTest, ScalarMultiply_ScalarTimesMatrix) {
  using VLA::Matrix;
  const Matrix<int, 2, 2> a{std::array<int, 4>{1, 2, 3, 4}};
  const auto b = 3 * a;

  EXPECT_EQ(b.A, (std::array<int, 4>{3, 6, 9, 12}));
}

TEST(MatrixTest, MatrixMultiply_IdentityActsAsIdentity) {
  using VLA::Matrix;
  const Matrix<int, 2, 2> I{std::array<int, 4>{1, 0, 0, 1}};
  const Matrix<int, 2, 2> A{std::array<int, 4>{5, 6, 7, 8}};

  const auto left = I * A;
  const auto right = A * I;

  EXPECT_EQ(left.A, A.A);
  EXPECT_EQ(right.A, A.A);
}

TEST(MatrixTest, MatrixMultiply_Known2x3Times3x2) {
  using VLA::Matrix;
  const Matrix<int, 2, 3> A{std::array<int, 6>{1, 2, 3, 4, 5, 6}};
  const Matrix<int, 3, 2> B{std::array<int, 6>{7, 8, 9, 10, 11, 12}};
  const auto C = A * B;

  EXPECT_EQ(C.A, (std::array<int, 4>{58, 64, 139, 154}));
}

TEST(MatrixTest, MatrixVectorMultiply_IdentityLeavesVectorUnchanged) {
  using VLA::Matrix;
  using VLA::Vector;
  const Matrix<float, 4, 4> I{std::array<float, 16>{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}};
  const Vector<float, 4> v{1.f, 2.f, 3.f, 1.f};
  const auto out = I * v;

  EXPECT_FLOAT_EQ(out[0], 1.f);
  EXPECT_FLOAT_EQ(out[1], 2.f);
  EXPECT_FLOAT_EQ(out[2], 3.f);
  EXPECT_FLOAT_EQ(out[3], 1.f);
}

TEST(MatrixTest, MatrixVectorMultiply_Known2x3TimesVec3) {
  using VLA::Matrix;
  using VLA::Vector;
  const Matrix<float, 2, 3> A{std::array<float, 6>{1, 2, 3, 4, 5, 6}};
  const Vector<float, 3> v{1.f, 1.f, 1.f};
  const auto out = A * v;

  EXPECT_FLOAT_EQ(out[0], 6.f);
  EXPECT_FLOAT_EQ(out[1], 15.f);
}

TEST(MatrixTest, Iterators_BeginEndSpanAllElements) {
  using VLA::Matrix;
  const Matrix<int, 2, 3> a{std::array<int, 6>{1, 2, 3, 4, 5, 6}};

  const auto count = static_cast<std::size_t>(std::distance(a.begin(), a.end()));
  EXPECT_EQ(count, a.A.size());

  const int sum_it = std::accumulate(a.begin(), a.end(), 0);
  EXPECT_EQ(sum_it, 21);
}