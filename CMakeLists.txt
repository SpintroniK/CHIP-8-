cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(CHIP8pp VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

add_executable(chip8pp Source/main.cpp)

target_compile_options(
    chip8pp
    PUBLIC
    -std=c++20
    -O3
    # -Wall
    -Werror
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wno-old-style-cast
    -Wcast-align
    # -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wdouble-promotion
    -Wformat=2
    # -Wno-unused-variable
    -Wno-ignored-attributes
    -pedantic
)


target_link_libraries(chip8pp sfml-graphics)