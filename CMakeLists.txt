cmake_minimum_required(VERSION 3.28)
add_compile_options(-Wall -Wextra -Wextra -pedantic-errors -Wconversion -Wsign-conversion)
add_compile_options(-O2)


project("One Billion Row Challenge Personal")
add_executable(main src/main.cpp src/naive.cpp)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)