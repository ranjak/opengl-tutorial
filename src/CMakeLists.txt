cmake_minimum_required(VERSION 3.0)

add_subdirectory(tutorial_framework)

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
  renderer/shader.cpp
  renderer/util.cpp
  renderer/transform.cpp
)

set(INCLUDE_RENDERER
  renderer/primitives.hpp
  renderer/shader.hpp
  renderer/util.hpp
  renderer/transform.hpp
)

set(SRC_TUTORIAL
  tutorial/tutorial.cpp
  tutorial/rotatingtriangle.cpp
  tutorial/movingtriangle.cpp
  tutorial/movingtriangleshader.cpp
  tutorial/orthographicprism.cpp
  tutorial/manualperspective.cpp
  tutorial/matrixperspective.cpp
  tutorial/indexeddrawing.cpp
  tutorial/basevertex.cpp
  tutorial/depthbuffering.cpp
  tutorial/depthclamping.cpp
)

set(INCLUDE_TUTORIAL
  tutorial/tutorial.hpp
  tutorial/rotatingtriangle.hpp
  tutorial/movingtriangle.hpp
  tutorial/movingtriangleshader.hpp
  tutorial/orthographicprism.hpp
  tutorial/manualperspective.hpp
  tutorial/matrixperspective.hpp
  tutorial/indexeddrawing.hpp
  tutorial/basevertex.hpp
  tutorial/depthbuffering.hpp
  tutorial/depthclamping.hpp
)

set(TUTORIAL_PRIMITIVES
  tutorial/primitives/indexedwedges.hpp
  tutorial/primitives/pointyshape.hpp
  tutorial/primitives/rectangle.hpp
)

set(SRC_TUTORIAL_POSITIONING
  tutorial/positioning/translation.cpp
  tutorial/positioning/hierarchy.cpp
  tutorial/positioning/nodehierarchy.cpp
  tutorial/positioning/node.cpp
)

set(INCLUDE_TUTORIAL_POSITIONING
  tutorial/positioning/translation.hpp
  tutorial/positioning/matrixstack.hpp
  tutorial/positioning/hierarchy.hpp
  tutorial/positioning/nodehierarchy.hpp
  tutorial/positioning/node.hpp
)

set(SRC_TUTORIAL_WORLD
  tutorial/world/worldscene.cpp
)

set(INCLUDE_TUTORIAL_WORLD
  tutorial/world/worldscene.hpp
)

set(SHADERS
  ../shaders/vertex.glsl
  ../shaders/fragment.glsl
  ../shaders/colored-triangle.vert
  ../shaders/colored-triangle.frag
  ../shaders/positionoffset.vert
  ../shaders/positionoffset-calc.vert
  ../shaders/colored-triangle-calc.frag
  ../shaders/manualperspective.vert
  ../shaders/matrixperspective.vert
  ../shaders/matrixperspective-vec3offset.vert
  ../shaders/modeltoclip.vert
)

set(SHADERS_WORLD
  ../shaders/world/PosOnlyWorldTransform.vert
  ../shaders/world/PosColorWorldTransform.vert
  ../shaders/world/ColorUniform.frag
  ../shaders/world/ColorPassthrough.frag
  ../shaders/world/ColorMultUniform.frag
)

source_group(system FILES ${SRC_SYSTEM} ${INCLUDE_SYSTEM})
source_group(renderer FILES ${SRC_RENDERER} ${INCLUDE_RENDERER})
source_group(tutorial FILES ${SRC_TUTORIAL} ${INCLUDE_TUTORIAL})
source_group(shaders FILES ${SHADERS})

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

add_executable(ogl-app WIN32
  ${SRC_SYSTEM}
  ${INCLUDE_SYSTEM}
  ${SRC_RENDERER}
  ${INCLUDE_RENDERER}
  ${SRC_TUTORIAL}
  ${INCLUDE_TUTORIAL}
  ${SRC_TUTORIAL_POSITIONING}
  ${INCLUDE_TUTORIAL_POSITIONING}
  ${SRC_TUTORIAL_WORLD}
  ${INCLUDE_TUTORIAL_WORLD}
  ${TUTORIAL_PRIMITIVES}
  ${SHADERS}
  ${SHADERS_WORLD}
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
  tutorial
)

target_link_libraries(ogl-app
  tutorial_framework
  glm
  glfw
  glad
)

# Install targets

# Binaries
install(TARGETS ogl-app RUNTIME DESTINATION ".")
# Resources
install(DIRECTORY ../shaders DESTINATION "." USE_SOURCE_PERMISSIONS)
