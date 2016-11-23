# OpenGL app dependencies

find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

# Use glad for OpenGL extension loading

option(OGLTUTOR_BUNDLED_GLAD "Use the bundled Glad OpenGL extension loader. Set to OFF if Glad is already installed on your system." ON)

if (OGLTUTOR_BUNDLED_GLAD)

  set(GLAD_PROFILE "compatibility" CACHE INTERNAL "OpenGL profile" FORCE)
  set(GLAD_API "gl=3.3" CACHE INTERNAL "API type/version pairs, like \"gl=3.2,gles=\", no version means latest" FORCE)
  set(GLAD_GENERATOR "c" CACHE INTERNAL "Language to generate the binding for" FORCE)
  set(GLAD_EXTENSIONS "" CACHE INTERNAL "Path to extensions file or comma separated list of extensions, if missing all extensions are included" FORCE)
  set(GLAD_SPEC "gl" CACHE INTERNAL "Name of the spec" FORCE)
  set(GLAD_NO_LOADER ON CACHE INTERNAL "No loader" FORCE)

  add_subdirectory("3rdparty/glad" ${CMAKE_CURRENT_BINARY_DIR}/glad)

else(OGLTUTOR_BUNDLED_GLAD)

  message(FATAL_ERROR "Using system Glad is not supported yet.")

endif(OGLTUTOR_BUNDLED_GLAD)
