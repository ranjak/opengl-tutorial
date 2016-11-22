cmake_minimum_required(VERSION 2.8)

set(SRC_SYSTEM
  system/main.cpp
  system/mainloop.cpp
  system/log.cpp
  system/system.cpp
  system/glfw/glfwwindow.cpp
)

set(INCLUDE_SYSTEM
  system/mainloop.hpp
  system/log.hpp
  system/system.hpp
  system/window.hpp
  system/glfw/glfwwindow.hpp
)

source_group(system FILES ${SRC_SYSTEM} ${INCLUDE_SYSTEM})

add_executable(ogl-app WIN32
  ${SRC_SYSTEM}
  ${INCLUDE_SYSTEM}
)

include_directories(
  system
  ${GLM_INCLUDE_DIRS}
)


