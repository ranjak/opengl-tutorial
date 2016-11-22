#ifndef WINDOWGLFW_H
#define WINDOWGLFW_H

#include "window.hpp"
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

class WindowGLFW : public Window
{
public:
  static void setGLversion(int major, int minor);

  WindowGLFW(int width=640, int height=480, std::string title="Window");

private:
  std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> mWindow;
};

#endif // WINDOWGLFW_H
