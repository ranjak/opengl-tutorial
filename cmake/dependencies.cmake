# OpenGL app dependencies

find_package(glfw3 REQUIRED)

# Use glm for 3D math functionality

option(OGLTUTOR_SYSTEM_GLM "Use the system-provided glm library." ON)

if (OGLTUTOR_SYSTEM_GLM)

  find_package(glm REQUIRED)

else(OGLTUTOR_SYSTEM_GLM)

  # Grab glm from its git repository
  find_program(GIT git)
  if (NOT GIT)
    message(FATAL_ERROR "Git is required to download glm, but was not found on the system.")
  endif()

  if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm")

    execute_process(COMMAND "${GIT}" clone "https://github.com/g-truc/glm"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty"
    )
  else()
    execute_process(COMMAND "${GIT}" pull
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm"
    )
  endif()

  add_subdirectory("3rdparty/glm" ${CMAKE_CURRENT_BINARY_DIR}/glm)

endif(OGLTUTOR_SYSTEM_GLM)


# Use glad for OpenGL extension loading

option(OGLTUTOR_SYSTEM_GLAD "Set to ON if the Glad OpenGL extension loader is installed on your system. Set to OFF to use the bundled Glad." OFF)

if (NOT OGLTUTOR_SYSTEM_GLAD)

  find_program(GIT git)
  if (NOT GIT)
    message(FATAL_ERROR "Git is required to download glad, but was not found on the system.")
  endif()

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

  message(FATAL_ERROR "Using system Glad is not supported yet.")

endif(NOT OGLTUTOR_SYSTEM_GLAD)
