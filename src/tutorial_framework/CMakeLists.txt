set(SRC
  Mesh.cpp
  )

set(INCLUDE
  Mesh.h
  )

add_library(tutorial_framework OBJECT
  ${SRC}
  ${INCLUDE}
  )

target_include_directories(tutorial_framework
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
  PRIVATE ${GLAD_INCLUDE_DIRS}
  )

target_compile_options(tutorial_framework PRIVATE -Wno-deprecated)

# Prevent glad from including windows.h
if (WIN32)
  target_compile_definitions(tutorial_framework PRIVATE -DAPIENTRY=__stdcall)
endif(WIN32)
