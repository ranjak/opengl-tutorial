#include "system.hpp"
#include "log.hpp"
#include "glfw/windowglfw.hpp"
#include <GLFW/glfw3.h>
#include <cassert>

std::unique_ptr<System> System::sysInstance = nullptr;
std::chrono::time_point<std::chrono::steady_clock> System::initTime;


bool System::init()
{
  if (sysInstance) {
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

  sysInstance.reset(new System);
  sysInstance->mainLoop.run();

  return true;
}

std::chrono::milliseconds System::now()
{
  assert(sysInstance);

  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - initTime);
}

System::System() :
  window(new WindowGLFW),
  mainLoop()
{
}

System::~System()
{
  glfwTerminate();
}
