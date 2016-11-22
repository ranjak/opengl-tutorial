#include "windowglfw.hpp"
#include "log.hpp"


void WindowGLFW::setGLversion(int major, int minor)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

WindowGLFW::WindowGLFW(int width, int height, std::string title) :
  mWindow(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr), glfwDestroyWindow)
{
  if (!mWindow) {
    Log::error("WindowGLFW: Window creation failed");
  }
}
