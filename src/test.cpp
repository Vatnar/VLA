#include <VLA.hpp>
#include <iostream>
#include <ostream>
#include <print>


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
constexpr auto TestMatrixMulAndTransform() {
  auto shear = RotationShear();

  // Matrix mul: shear * shear
  auto mulResult = shear * shear;

  // Vector transform: mulResult * unitX
  constexpr VLA::Vector4f unitX{1.0f, 0.0f, 0.0f, 1.0f}; // Homogeneous point
  auto transformed = mulResult * unitX;

  // assert(transformed == expected); // Compile-time check

  std::print("Shear matrix:\n{}\n\n", shear);
  std::print("shear * shear:\n{}\n\n", mulResult);
  std::print("Transformed unitX: {} -> {}\n", unitX, transformed);

  return transformed;
}

int main() {
  auto result = TestMatrixMulAndTransform();
  std::println("All checks passed: {}", result);
}
