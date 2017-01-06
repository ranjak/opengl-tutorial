#include "system.hpp"
#include "log.hpp"
#include "glfw/windowglfw.hpp"
#include <GLFW/glfw3.h>
#include <cassert>
#include <memory>

std::chrono::time_point<std::chrono::steady_clock> System::initTime;

namespace
{
// Dummy that ensures proper cleanup when the program ends.
std::unique_ptr<System> systemDummy;
}

System::System()
{}

System::~System()
{
  glfwTerminate();
}

bool System::init()
{
  if (systemDummy) {
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
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  systemDummy.reset(new System);
  return true;
}

ogl::time System::now()
{
  assert(systemDummy);

  return std::chrono::duration_cast<ogl::time>(std::chrono::steady_clock::now() - initTime);
}

void System::pollEvents()
{
  assert(systemDummy);

  glfwPollEvents();
}

void System::cleanup()
{
  systemDummy.release();
}
