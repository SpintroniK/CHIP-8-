cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(CHIP8pp VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)


add_executable(chip8pp Source/main.cpp)

set(CMAKE_VERBOSE_MAKEFILE OFF)

if(EMSCRIPTEN)
    message("Compiling with emsdk.")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_target_properties(chip8pp PROPERTIES LINK_FLAGS "-sUSE_SDL=2 -sDISABLE_EXCEPTION_CATCHING=1 -sPROXY_TO_PTHREAD -sUSE_PTHREADS -sTOTAL_MEMORY=32MB -O3 -sENVIRONMENT=\"web,worker\" --embed-file roms --closure 1 -fno-rtti -flto")
    target_compile_options(chip8pp PUBLIC "-pthread")
else()
    target_link_libraries(chip8pp Threads::Threads SDL2)
endif()

include_directories(${SDL2_INCLUDE_DIR})
