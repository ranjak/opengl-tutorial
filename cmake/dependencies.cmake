# OpenGL app dependencies

# Check whether libraries are installed on the system to set initial option values
if (NOT DEFINED OGLTUTOR_SYSTEM_GLFW)
  find_package(glfw3 QUIET)
endif()
if (NOT DEFINED OGLTUTOR_SYSTEM_GLM)
  find_package(glm QUIET)
endif()
if (NOT DEFINED OGLTUTOR_SYSTEM_GLAD)
  find_package(glad QUIET)
endif()

# Use GLFW for window, OpenGL context and input management
option(OGLTUTOR_SYSTEM_GLFW "Use the system-provided GLFW library." ${glfw3_FOUND})

# Use glm for 3D math functionality
option(OGLTUTOR_SYSTEM_GLM "Use the system-provided glm library." ${glm_FOUND})

# Use glad for OpenGL extension loading
option(OGLTUTOR_SYSTEM_GLAD "Set to ON if the Glad OpenGL extension loader is installed on your system. Set to OFF to use the bundled Glad." ${glad_FOUND})


# If we are going to build dependencies, make them static
set(BUILD_SHARED_LIBS OFF)

# Git is needed to manage dependencies
if (NOT (OGLTUTOR_SYSTEM_GLFW AND OGLTUTOR_SYSTEM_GLM AND OGLTUTOR_SYSTEM_GLAD))
  find_program(GIT git)
  if (NOT GIT)
    message(FATAL_ERROR "Git is required to manage dependencies, but was not found on the system.")
  endif()
endif()


if (OGLTUTOR_SYSTEM_GLFW)

  message(STATUS "Using system-provided GLFW")
  find_package(glfw3 REQUIRED)

else(OGLTUTOR_SYSTEM_GLFW)

  message(STATUS "Building GLFW from git")
  if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glfw")

    execute_process(COMMAND "${GIT}" clone "https://github.com/glfw/glfw"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty"
    )
  else()
    execute_process(COMMAND "${GIT}" pull
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glfw"
    )
  endif()

  execute_process(COMMAND "${GIT}" checkout latest
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glfw"
  )

  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

  if (OGLTUTOR_STATIC)
    set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
  else()
    set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE BOOL "" FORCE)
  endif()

  add_subdirectory("3rdparty/glfw" ${CMAKE_CURRENT_BINARY_DIR}/glfw)

endif(OGLTUTOR_SYSTEM_GLFW)


if (OGLTUTOR_SYSTEM_GLM)

  message(STATUS "Using system-provided glm")
  find_package(glm REQUIRED)

else(OGLTUTOR_SYSTEM_GLM)

  message(STATUS "Using glm from git")
  if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm")

    execute_process(COMMAND "${GIT}" clone "https://github.com/ranjak/glm"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty"
    )
  else()
    execute_process(COMMAND "${GIT}" pull
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm"
    )
  endif()

  set(GLM_INSTALL_ENABLE OFF CACHE BOOL "" FORCE)

  add_subdirectory("3rdparty/glm" ${CMAKE_CURRENT_BINARY_DIR}/glm)

endif(OGLTUTOR_SYSTEM_GLM)


if (NOT OGLTUTOR_SYSTEM_GLAD)

  message(STATUS "Using glad from git")
  execute_process(COMMAND "${GIT}" submodule update --init -- 3rdparty/glad
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  )

  set(GLAD_PROFILE "compatibility" CACHE INTERNAL "OpenGL profile" FORCE)
  set(GLAD_API "gl=3.3" CACHE INTERNAL "API type/version pairs, like \"gl=3.2,gles=\", no version means latest" FORCE)
  set(GLAD_GENERATOR "c" CACHE INTERNAL "Language to generate the binding for" FORCE)
  set(GLAD_EXTENSIONS "" CACHE INTERNAL "Path to extensions file or comma separated list of extensions, if missing all extensions are included" FORCE)
  set(GLAD_SPEC "gl" CACHE INTERNAL "Name of the spec" FORCE)
  set(GLAD_NO_LOADER ON CACHE INTERNAL "No loader" FORCE)

  add_subdirectory("3rdparty/glad" ${CMAKE_CURRENT_BINARY_DIR}/glad)

else(NOT OGLTUTOR_SYSTEM_GLAD)

  message(FATAL_ERROR "Using system Glad is not supported yet")

endif(NOT OGLTUTOR_SYSTEM_GLAD)
