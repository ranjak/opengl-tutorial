#ifndef WINDOWGLFW_H
#define WINDOWGLFW_H

#include "window.hpp"
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <vector>

class WindowGLFW : public Window
{
public:
  static void setGLversion(int major, int minor);

  WindowGLFW(int width=640, int height=480, const std::string& title="Window");

  void setCloseCallback(void (*callback)(Window*)) override;

  void setFramebufferResizeCallback(void (*callback)(Window*, int w, int h)) override;

  void makeContextCurrent() override;

  void setSwapInterval(int interval) override;

  void swapBuffers() override;

  std::pair<int, int> getSize() override;

  std::pair<int, int> getFramebufferSize() override;

  void addKeyCallback(KeyCallback callback);

  int getModifierKeys() override;

  void setMouseMovementCallback(MouseMvtCallback callback) override;
  void setMouseButtonCallback(MouseButtonCallback callback) override;
  void setScrollCallback(ScrollCallback callback) override;

  virtual std::pair<double, double> getCursorPos() override;

private:
  void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
  std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> mWindow;
  void (*mCloseCallback)(Window*);
  void (*mFBResizeCallback)(Window*, int, int);

  std::vector<KeyCallback> mKeyCallbacks;
  int mModifierKeys;
  MouseMvtCallback mMousePosCb;
  MouseButtonCallback mMouseButtonCb;
  ScrollCallback mScrollCb;
};

#endif // WINDOWGLFW_H
