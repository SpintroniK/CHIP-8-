cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(CHIP8pp VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(chip8pp Source/main.cpp)

set(CMAKE_VERBOSE_MAKEFILE OFF)

if(EMSCRIPTEN)
    message("-- EMSCRIPTEN detected")
	set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set_target_properties(chip8pp PROPERTIES LINK_FLAGS "-sUSE_SDL=2 -sPROXY_TO_PTHREAD -sUSE_PTHREADS -s PTHREAD_POOL_SIZE=4 -sTOTAL_MEMORY=32MB -O3 -sENVIRONMENT=\"web,worker\" --closure 1 -fno-rtti -flto -fwasm-exceptions")
    target_compile_options(chip8pp PUBLIC "-pthread")
else()
    find_package(Threads REQUIRED)
    find_package(SDL2 REQUIRED)
    target_link_libraries(chip8pp Threads::Threads SDL2)
endif()

include_directories(${SDL2_INCLUDE_DIR})
