project(opengl-tutorial)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Global options

option(OGLTUTOR_STATIC "Build opengl-tutorial statically (on Unix, it will still be dynamically linked to glibc and window manager libs)" OFF)

if (OGLTUTOR_STATIC)

  if (UNIX OR MINGW)
    list(APPEND CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
  elseif (MSVC)
    # Use /MT (static runtime) instead of /MD project-wide
    foreach (flag
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL
        CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        )

      if (${flag} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
      endif()
      if (${flag} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
      endif()

    endforeach()
  endif()
  
endif(OGLTUTOR_STATIC)

# Find and/or build dependencies

include(dependencies)

# Configuration

include(utilities)
use_cxx_std(14)

# Warnings
if (NOT MSVC)
  list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
endif()

if (NOT CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
  add_definitions(-DNDEBUG)
endif()

add_subdirectory(src)
