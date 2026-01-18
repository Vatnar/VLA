#include <VLA.hpp>
#include <print>

int main() {
  using namespace VLA;
  Vector2f v1 = {3, -8};
  Vector2f v2 = {1, 1};

  Vector2f v3 = v1 + static_cast<float>(3) * v2;
  Vector2f v4 = v1 - v2;

  std::print("v3: {}\nv4: {}", v3, v4);
}
