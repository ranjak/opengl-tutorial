macro(use_cxx_std CXX_STD)

  if (CMAKE_VERSION VERSION_LESS "3.1")

    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++${CXX_STD}" COMPILER_SUPPORTS_CXX${CXX_STD})

    if (COMPILER_SUPPORTS_CXX${CXX_STD})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CXX_STD}")
    else()
      message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++${CXX_STD} support. Please use a different C++ compiler.")
    endif()

  else (CMAKE_VERSION VERSION_LESS "3.1")
    set (CMAKE_CXX_STANDARD ${CXX_STD})
  endif (CMAKE_VERSION VERSION_LESS "3.1")

endmacro(use_cxx_std)

