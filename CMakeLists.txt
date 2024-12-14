cmake_minimum_required(VERSION 3.28)
add_compile_options(-Wall -Wextra -Wextra -pedantic-errors -Wconversion -Wsign-conversion)
add_compile_options(-O2)


project("One Billion Row Challenge Personal")
add_executable(main src/main.cpp src/memory_map.cpp src/better.cpp src/naive.cpp)

set(unordered_dense_DIR "/home/shawn/unordered_dense_install/lib/cmake/unordered_dense")
find_package(unordered_dense CONFIG REQUIRED)
target_link_libraries(main unordered_dense::unordered_dense)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)