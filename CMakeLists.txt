cmake_minimum_required(VERSION 3.25...4.0)
project(VLA)

set(CMAKE_CXX_STANDARD 23)

add_library(VLA STATIC src/VLA.cpp src/Vector.cpp src/Matrix.cpp)
target_include_directories(VLA PUBLIC include)

enable_testing()

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")
add_executable(VLAGTest ${TEST_SOURCES})

target_include_directories(VLAGTest PRIVATE
        include
        ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(VLAGTest PRIVATE
        VLA
        GTest::gtest
)
add_test(NAME VLAGTest COMMAND VLAGTest)