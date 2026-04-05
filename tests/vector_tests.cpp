#include <VLA/Vector.hpp>
#include <gtest/gtest.h>
#include <array>

using VLA::Vector2f;
using VLA::Vector3f;
using VLA::Vector4f;
using VLA::Vector2i;
using VLA::Vector3i;

TEST(VectorTest, Addition) {
  Vector3f a{1.0f, 2.0f, 3.0f};
  Vector3f b{4.0f, 5.0f, 6.0f};
  Vector3f c = a + b;

  EXPECT_FLOAT_EQ(c[0], 5.0f);
  EXPECT_FLOAT_EQ(c[1], 7.0f);
  EXPECT_FLOAT_EQ(c[2], 9.0f);
}

TEST(VectorTest, Subtraction) {
  Vector3f a{4.0f, 5.0f, 6.0f};
  Vector3f b{1.0f, 2.0f, 3.0f};
  Vector3f c = a - b;

  EXPECT_FLOAT_EQ(c[0], 3.0f);
  EXPECT_FLOAT_EQ(c[1], 3.0f);
  EXPECT_FLOAT_EQ(c[2], 3.0f);
}

TEST(VectorTest, ScalarMultiply_VectorTimesScalar) {
  Vector3f a{1.0f, 2.0f, 3.0f};
  Vector3f b = a * 2.0f;

  EXPECT_FLOAT_EQ(b[0], 2.0f);
  EXPECT_FLOAT_EQ(b[1], 4.0f);
  EXPECT_FLOAT_EQ(b[2], 6.0f);
}

TEST(VectorTest, ScalarMultiply_ScalarTimesVector) {
  Vector3f a{1.0f, 2.0f, 3.0f};
  Vector3f b = 3.0f * a;

  EXPECT_FLOAT_EQ(b[0], 3.0f);
  EXPECT_FLOAT_EQ(b[1], 6.0f);
  EXPECT_FLOAT_EQ(b[2], 9.0f);
}

TEST(VectorTest, DotProduct) {
  Vector3f a{1.0f, 2.0f, 3.0f};
  Vector3f b{4.0f, 5.0f, 6.0f};
  float dot = Vector3f::Dot(a, b);

  EXPECT_FLOAT_EQ(dot, 32.0f);
}

TEST(VectorTest, DotProduct_OrthogonalVectors) {
  Vector3f a{1.0f, 0.0f, 0.0f};
  Vector3f b{0.0f, 1.0f, 0.0f};
  float dot = Vector3f::Dot(a, b);

  EXPECT_FLOAT_EQ(dot, 0.0f);
}

TEST(VectorTest, DotProduct_SameVector) {
  Vector3f a{3.0f, 4.0f, 0.0f};
  float dot = Vector3f::Dot(a, a);

  EXPECT_FLOAT_EQ(dot, 25.0f);
}

TEST(VectorTest, CrossProduct) {
  Vector3f a{1.0f, 0.0f, 0.0f};
  Vector3f b{0.0f, 1.0f, 0.0f};
  Vector3f c = Vector3f::Cross(a, b);

  EXPECT_FLOAT_EQ(c[0], 0.0f);
  EXPECT_FLOAT_EQ(c[1], 0.0f);
  EXPECT_FLOAT_EQ(c[2], 1.0f);
}

TEST(VectorTest, CrossProduct_Anticommutative) {
  Vector3f a{1.0f, 0.0f, 0.0f};
  Vector3f b{0.0f, 1.0f, 0.0f};
  Vector3f c1 = Vector3f::Cross(a, b);
  Vector3f c2 = Vector3f::Cross(b, a);

  EXPECT_FLOAT_EQ(c1[0], -c2[0]);
  EXPECT_FLOAT_EQ(c1[1], -c2[1]);
  EXPECT_FLOAT_EQ(c1[2], -c2[2]);
}

TEST(VectorTest, Length) {
  Vector3f v{3.0f, 4.0f, 0.0f};
  float len = v.Length();

  EXPECT_FLOAT_EQ(len, 5.0f);
}

TEST(VectorTest, Length_ZeroVector) {
  Vector3f v{0.0f, 0.0f, 0.0f};
  float len = v.Length();

  EXPECT_FLOAT_EQ(len, 0.0f);
}

TEST(VectorTest, Length_UnitVector) {
  Vector3f v{1.0f, 0.0f, 0.0f};
  float len = v.Length();

  EXPECT_FLOAT_EQ(len, 1.0f);
}

TEST(VectorTest, Equality) {
  Vector3f a{1.0f, 2.0f, 3.0f};
  Vector3f b{1.0f, 2.0f, 3.0f};
  Vector3f c{1.0f, 2.0f, 4.0f};

  EXPECT_EQ(a, b);
  EXPECT_NE(a, c);
}

TEST(VectorTest, AccessOperator) {
  Vector3f v{1.0f, 2.0f, 3.0f};

  EXPECT_FLOAT_EQ(v[0], 1.0f);
  EXPECT_FLOAT_EQ(v[1], 2.0f);
  EXPECT_FLOAT_EQ(v[2], 3.0f);

  v[0] = 10.0f;
  EXPECT_FLOAT_EQ(v[0], 10.0f);
}

TEST(VectorTest, DifferentSizes) {
  Vector2f v2{1.0f, 2.0f};
  EXPECT_FLOAT_EQ(v2[0], 1.0f);
  EXPECT_FLOAT_EQ(v2[1], 2.0f);

  Vector4f v4{1.0f, 2.0f, 3.0f, 4.0f};
  EXPECT_FLOAT_EQ(v4[0], 1.0f);
  EXPECT_FLOAT_EQ(v4[3], 4.0f);
}

TEST(VectorTest, IntegerVector) {
  Vector3i a{1, 2, 3};
  Vector3i b{4, 5, 6};
  Vector3i c = a + b;

  EXPECT_EQ(c[0], 5);
  EXPECT_EQ(c[1], 7);
  EXPECT_EQ(c[2], 9);

  int dot = Vector3i::Dot(a, b);
  EXPECT_EQ(dot, 32);
}