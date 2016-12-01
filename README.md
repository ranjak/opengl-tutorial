# OpenGL Tutorial

A cross-platform C++ OpenGL sample application.

## Building from source

This project uses the CMake build system.
Linux with GCC and Windows with MSVC are supported,
but other systems and compilers might work.

### Prerequisites

* CMake >= 3.0
* A C++ compiler that supports C++14 (tested with GCC 6 and MSVC 14 aka VS 2015)
* Python >= 2.7
* Git

### Optional Dependencies

The app uses __GLFW__, __glm__ and __glad__.
You have the choice to either install development libraries for them,
or let the CMake script download and build them.

The CMake script will first try to find them on your system.
If they cannot be found, the script will build them from source.

__Linux users__: Building GLFW from source requires X.Org development files.
On Ubuntu, these correspond to the `xorg-dev` package group.

### Build Configuration

In addition to the usual CMake variables (`CMAKE_INSTALL_PREFIX`, etc.),
there are `OGLTUTOR_` variables that you can set to tweak build options.

The `OGLTUTOR_SYSTEM_` options allow you to toggle between using system-provided dependencies
or building them from source.

`OGLTUTOR_STATIC` allows building the application statically,
provided that the dependencies are supplied as static libraries.
A quick way to make sure this is the case is to let the script build every dependency
instead of using system-provided ones.

On Windows, it will result in a completely static executable.
On Linux, the executable will still be dynamically linked to Glibc and Xorg libraries,
but with static libstdc++/libgcc, making it (hopefully) portable.

## Running

When launching the `ogl-app` executable, the `shaders` folder has to be in your working directory.
