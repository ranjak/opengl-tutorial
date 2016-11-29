cmake_minimum_required(VERSION 2.8)

set(SRC_SYSTEM
  system/main.cpp
  system/mainloop.cpp
  system/log.cpp
  system/system.cpp
  system/glfw/windowglfw.cpp
)

set(INCLUDE_SYSTEM
  system/mainloop.hpp
  system/log.hpp
  system/system.hpp
  system/window.hpp
  system/glfw/windowglfw.hpp
)

set(SRC_RENDERER
  renderer/renderer.cpp
  renderer/shader.cpp
  renderer/util.cpp
)

set(INCLUDE_RENDERER
  renderer/renderer.hpp
  renderer/primitives.hpp
  renderer/shader.hpp
  renderer/util.hpp
)

source_group(system FILES ${SRC_SYSTEM} ${INCLUDE_SYSTEM})

add_executable(ogl-app WIN32
  ${SRC_SYSTEM}
  ${INCLUDE_SYSTEM}
  ${SRC_RENDERER}
  ${INCLUDE_RENDERER}
)

target_include_directories(ogl-app PRIVATE
  system
  renderer
  ${GLM_INCLUDE_DIRS}
)

target_link_libraries(ogl-app
  glfw
  glad
)

