cmake_minimum_required(VERSION 4.1)
project(VLA)

set(CMAKE_CXX_STANDARD 23)

add_library(VLA STATIC src/VLA.cpp)
target_include_directories(VLA PUBLIC include)


# during dev
add_executable(test src/test.cpp
        include/VLA/Vector.h
        src/Vector.cpp)
target_link_libraries(test PRIVATE VLA)