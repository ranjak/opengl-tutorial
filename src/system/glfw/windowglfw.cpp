#include "windowglfw.hpp"
#include "log.hpp"


void WindowGLFW::setGLversion(int major, int minor)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

WindowGLFW::WindowGLFW(int width, int height, const std::string& title) :
  mWindow(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr), glfwDestroyWindow),
  mCloseCallback(nullptr),
  mFBResizeCallback(nullptr)
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

void WindowGLFW::setFramebufferResizeCallback(void (*callback)(Window*, int, int))
{
  mFBResizeCallback = callback;

  glfwSetFramebufferSizeCallback(mWindow.get(), [](GLFWwindow* win, int w, int h)
  {
    WindowGLFW* thisWindow = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(win));
    if (thisWindow->mFBResizeCallback) {
      thisWindow->mFBResizeCallback(thisWindow, w, h);
    }
  });
}

void WindowGLFW::makeContextCurrent()
{
  glfwMakeContextCurrent(mWindow.get());
}

void WindowGLFW::setSwapInterval(int interval)
{
  glfwMakeContextCurrent(mWindow.get());
  glfwSwapInterval(interval);
}

void WindowGLFW::swapBuffers()
{
  glfwSwapBuffers(mWindow.get());
}

std::pair<int, int> WindowGLFW::getSize()
{
  std::pair<int, int> size;
  glfwGetWindowSize(mWindow.get(), &size.first, &size.second);

  return size;
}

std::pair<int, int> WindowGLFW::getFramebufferSize()
{
  std::pair<int, int> size;
  glfwGetFramebufferSize(mWindow.get(), &size.first, &size.second);

  return size;
}
