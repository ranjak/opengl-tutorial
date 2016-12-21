set(SRC
  Mesh.cpp
  )

set(INCLUDE
  Mesh.h
  )

add_library(tutorial_framework STATIC
  ${SRC}
  ${INCLUDE}
  )

target_link_libraries(tutorial_framework
  PRIVATE glm glad
  )

target_include_directories(tutorial_framework
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
  )

# rapidxml uses an include that is marked as deprecated at least on gcc 6.2
if (NOT MSVC)
  target_compile_options(tutorial_framework PRIVATE -Wno-deprecated)
endif(NOT MSVC)

# Prevent glad from including windows.h
if (WIN32)
  target_compile_definitions(tutorial_framework PRIVATE -DAPIENTRY=__stdcall)
endif(WIN32)
