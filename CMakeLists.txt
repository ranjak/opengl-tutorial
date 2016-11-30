project(opengl-tutorial)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Global options

option(OGLTUTOR_STATIC "Build opengl-tutorial statically (on Unix, it will still be dynamically linked to glibc and window manager libs)" OFF)

# Find and/or build dependencies

include(dependencies)

# Configuration

include(utilities)
use_cxx_std(14)

if (OGLTUTOR_STATIC AND UNIX)
  list(APPEND CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

# Warnings
if (NOT MSVC)
  list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
endif()

if (NOT CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    add_definitions(-DNDEBUG)
endif()

add_subdirectory(src)
