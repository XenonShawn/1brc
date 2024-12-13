cmake_minimum_required(VERSION 3.28)
add_compile_options(-Wall -Wextra -Wextra -pedantic-errors -Wconversion -Wsign-conversion)
add_compile_options(-O2)


project("One Billion Row Challenge Personal")
add_executable(main src/main.cpp src/better.cpp)

set(unordered_dense_DIR "/home/shawn/unordered_dense_install/lib/cmake/unordered_dense")
find_package(unordered_dense CONFIG REQUIRED)
target_link_libraries(main unordered_dense::unordered_dense)

find_package(Boost 1.83 REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_include_directories(main PRIVATE ${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found!")
endif()

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)