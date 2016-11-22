#include "system.hpp"
#include "log.hpp"
#include "glfw/windowglfw.hpp"
#include <GLFW/glfw3.h>
#include <cassert>

bool System::wasInit = false;
std::chrono::time_point<std::chrono::steady_clock> System::initTime;


bool System::init()
{
  if (wasInit) {
    Log::error("System already initialized.");
    return false;
  }

  initTime = std::chrono::steady_clock::now();

  glfwSetErrorCallback([](int /*error*/, const char* description)
  {
    Log::error(std::string("GLFW error: ") + description);
  });

  if (!glfwInit()) {
    return false;
  }

  WindowGLFW::setGLversion(3, 3);

  wasInit = true;
  return true;
}

ogl::time System::now()
{
  assert(wasInit);

  return std::chrono::duration_cast<ogl::time>(std::chrono::steady_clock::now() - initTime);
}

void System::pollEvents()
{
  assert(wasInit);

  glfwPollEvents();
}

void System::cleanup()
{
  if (!wasInit) {
    return;
  }

  glfwTerminate();
}
