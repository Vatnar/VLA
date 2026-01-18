#include <VLA.hpp>
#include <print>

int main() {
  using namespace VLA;
  Vector2f v1 = {3, -8};
  Vector2f v2 = {1, 1};

  Vector2f v3 = v1 + static_cast<float>(3) * v2;
  Vector2f v4 = v1 - v2;

  /*
   * [ 1 0   0 0 ] [ 1 ]   [ 1 ]
   * [ 0 1.4 0 0 ] [ 4 ] = [ 5.6 ]
   * [ 0 0   5 0 ] [ 4 ]   [ 20]
   * [ 0 0   0 1 ] [ 2 ]   [ 2 ]
   *
   */

  // clang-format off
  Matrix4x4f a ={
    1, 0,    0, 0,
    0, 1.4f, 0, 0,
    0, 0,    5, 0,
    0, 0,    0, 1
  };

  Matrix4x4f b = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
  };
  // clang-format on
  Vector4f m = {1, 4, 4, 2};
  Vector4f mPrime = b * (a * m);

  std::print("mPrime: {}", mPrime);
}
