#include "windowglfw.hpp"
#include "log.hpp"
#include <algorithm>

namespace
{

Window::Action toKeyAction(int glfwAction)
{
  switch (glfwAction) {
  case GLFW_PRESS:
    return Window::PRESS;
  case GLFW_RELEASE:
    return Window::RELEASE;
  case GLFW_REPEAT:
    return Window::KEYREPEAT;
  default:
    return Window::ACT_UNKNOWN;
  }
}

short toKeyMod(int glfwMods)
{
  short mods = 0;

  if (glfwMods & GLFW_MOD_SHIFT) {
    mods |= Window::MOD_SHIFT;
  }
  if (glfwMods & GLFW_MOD_ALT) {
    mods |= Window::MOD_ALT;
  }
  if (glfwMods & GLFW_MOD_CONTROL) {
    mods |= Window::MOD_CONTROL;
  }

  return mods;
}

} // namespace

const int Window::MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT;
const int Window::MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
const int Window::MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;

void WindowGLFW::setGLversion(int major, int minor)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

WindowGLFW::WindowGLFW(int width, int height, const std::string& title) :
  mWindow(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr), glfwDestroyWindow),
  mCloseCallback(nullptr),
  mFBResizeCallback(nullptr),
  mKeyCallbacks(),
  mModifierKeys(0),
  mMousePosCb(nullptr),
  mMouseButtonCb(nullptr),
  mScrollCb(nullptr)
{
  if (!mWindow) {
    Log::error("WindowGLFW: Window creation failed");
  }

  glfwSetWindowUserPointer(mWindow.get(), this);

  glfwSetKeyCallback(mWindow.get(), [](GLFWwindow* win, int key, int scan, int action, int mod) {

    WindowGLFW* thisWin = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(win));
    thisWin->mModifierKeys = toKeyMod(mod);

    for (KeyCallback callback : thisWin->mKeyCallbacks) {
      callback(thisWin, key, scan, toKeyAction(action), thisWin->mModifierKeys);
    }
  });

  glfwSetCursorPosCallback(mWindow.get(), [](GLFWwindow* win, double xpos, double ypos) {

    WindowGLFW* thisWin = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(win));
    thisWin->mMousePosCb(thisWin, xpos, ypos);
  });
  glfwSetMouseButtonCallback(mWindow.get(), [](GLFWwindow* win, int button, int action, int mod) {

    WindowGLFW* thisWin = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(win));
    thisWin->mMouseButtonCb(thisWin, button, toKeyAction(action), toKeyMod(mod));
  });
  glfwSetScrollCallback(mWindow.get(), [](GLFWwindow* win, double xoffset, double yoffset) {

    WindowGLFW* thisWin = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(win));
    thisWin->mScrollCb(thisWin, xoffset, yoffset);
  });

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

void WindowGLFW::addKeyCallback(KeyCallback callback)
{
  if (std::find(mKeyCallbacks.begin(), mKeyCallbacks.end(), callback) == mKeyCallbacks.end()) {
    mKeyCallbacks.push_back(callback);
  }
}

int WindowGLFW::getModifierKeys()
{
  return mModifierKeys;
}

void WindowGLFW::setMouseMovementCallback(Window::MouseMvtCallback callback)
{
  mMousePosCb = callback;
}

void WindowGLFW::setMouseButtonCallback(Window::MouseButtonCallback callback)
{
  mMouseButtonCb = callback;
}

void WindowGLFW::setScrollCallback(Window::ScrollCallback callback)
{
  mScrollCb = callback;
}

std::pair<double, double> WindowGLFW::getCursorPos()
{
  std::pair<double, double> pos;
  glfwGetCursorPos(mWindow.get(), &pos.first, &pos.second);

  return pos;
}
