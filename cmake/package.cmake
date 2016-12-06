# CPack configuration

if (WIN32)
  set(CPACK_GENERATOR "ZIP")
else()
  set(CPACK_GENERATOR "TGZ")
endif()

# Get version string
find_program(GIT git)
if (GIT)
  execute_process(
    COMMAND ${GIT} describe
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE COMMIT_DESCRIPTION
    )

  # Remove line skip from git output
  string(REGEX REPLACE "[\r\n]" "" COMMIT_DESCRIPTION "${COMMIT_DESCRIPTION}")

  set(CPACK_PACKAGE_VERSION "${COMMIT_DESCRIPTION}")

else()
  message(WARNING "Packaging: git not found, cannot deduce version")
  set(CPACK_PACKAGE_VERSION "")

endif()

set(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

include(CPack)
