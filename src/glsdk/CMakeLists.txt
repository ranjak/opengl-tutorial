set(SRC
  source/MatrixStack.cpp
  source/MousePoles.cpp
  )

add_library(glsdk STATIC
  ${SRC}
  )

target_link_libraries(glsdk
  PRIVATE glm
  )

target_include_directories(glsdk
  PUBLIC include
  )
