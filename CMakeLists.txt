cmake_minimum_required(VERSION 3.16)

project(chess_engine
        VERSION 0.1.0
        DESCRIPTION "A chess engine, built milestone by milestone"
        LANGUAGES CXX)

# --- Language level -------------------------------------------------------
# STANDARD_REQUIRED means "fail loudly if the compiler can't do C++17"
# instead of silently falling back. EXTENSIONS OFF asks for -std=c++17
# rather than -std=gnu++17, so GNU-only extensions won't compile and your
# code stays portable.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Default to a debuggable build ----------------------------------------
# Without this, CMake picks no build type at all: no -O flags AND no -g,
# which makes gdb useless. Override on the command line when you want to
# measure real search speed:  cmake -DCMAKE_BUILD_TYPE=Release ..
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# --- Sources --------------------------------------------------------------
# Every .cpp under src/ is compiled. CONFIGURE_DEPENDS makes the build
# re-scan for new files, so adding board.cpp in milestone 1 needs no edit
# here. (Globbing is mildly frowned on in large projects -- see the note in
# the chat; swapping to an explicit list later is a two-line change.)
file(GLOB_RECURSE CHESS_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(chess_engine ${CHESS_SOURCES})

# PRIVATE = "this target needs include/ on its header search path, but
# nothing that links against it does." Lets you write
# #include "board.hpp" instead of #include "../include/board.hpp".
target_include_directories(chess_engine PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}/include")

# --- Warnings -------------------------------------------------------------
# Treat these as a free tutor: -Wall -Wextra catches a large share of the
# mistakes a C programmer makes in C++. Read every one of them.
if(MSVC)
    target_compile_options(chess_engine PRIVATE /W4)
else()
    target_compile_options(chess_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()
