cmake_minimum_required(VERSION 3.0)

set(SRC_SYSTEM
  system/main.cpp
  system/mainloop.cpp
  system/log.cpp
  system/system.cpp
  system/sysutils.cpp
  system/glfw/windowglfw.cpp
)

set(INCLUDE_SYSTEM
  system/mainloop.hpp
  system/log.hpp
  system/system.hpp
  system/window.hpp
  system/sysutils.hpp
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
source_group(renderer FILES ${SRC_RENDERER} ${INCLUDE_RENDERER})

add_executable(ogl-app WIN32
  ${SRC_SYSTEM}
  ${INCLUDE_SYSTEM}
  ${SRC_RENDERER}
  ${INCLUDE_RENDERER}
)

# GLFW shouldn't include gl.h since we're using a loader (glad)
target_compile_definitions(ogl-app PRIVATE
  -DGLFW_INCLUDE_NONE
)

# Prevent glad from including windows.h
if (WIN32)
  target_compile_definitions(ogl-app PRIVATE -DAPIENTRY=__stdcall)
endif(WIN32)

# Use main() instead of WinMain() with MSVC
if (MSVC)
  set_target_properties(ogl-app PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif(MSVC)

target_include_directories(ogl-app PRIVATE
  system
  renderer
)

target_link_libraries(ogl-app
  glm
  glfw
  glad
)

