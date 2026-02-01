cmake_minimum_required(VERSION 3.25...4.0)
project(VLA)

set(CMAKE_CXX_STANDARD 23)

add_library(VLA STATIC src/VLA.cpp src/Vector.cpp src/Matrix.cpp)
target_include_directories(VLA PUBLIC include)

enable_testing()

add_executable(vla_tests
        tests/test_main.cpp
        tests/matrix_tests.cpp
)

target_include_directories(vla_tests PRIVATE
        include
        vendor
)
target_link_libraries(vla_tests VLA)
add_test(NAME vla_tests COMMAND vla_tests)
