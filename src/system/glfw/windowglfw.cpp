#include "windowglfw.hpp"
#include "log.hpp"


void WindowGLFW::setGLversion(int major, int minor)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

WindowGLFW::WindowGLFW(int width, int height, const std::__cxx11::string& title) :
  mWindow(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr), glfwDestroyWindow),
  mCloseCallback(nullptr)
{
  if (!mWindow) {
    Log::error("WindowGLFW: Window creation failed");
  }

  glfwSetWindowUserPointer(mWindow.get(), this);
}

void WindowGLFW::setCloseCallback(void (*callback)(Window*))
{
  mCloseCallback = callback;

  glfwSetWindowCloseCallback(mWindow.get(), [](GLFWwindow* w)
  {
    WindowGLFW* thisWindow = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(w));
    if (thisWindow->mCloseCallback) {
      thisWindow->mCloseCallback(thisWindow);
    }
  });
}
