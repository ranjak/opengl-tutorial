#include "system.hpp"
#include <cassert>

std::unique_ptr<System> System::sysInstance = nullptr;
std::chrono::time_point<std::chrono::steady_clock> System::initTime;

bool System::init()
{
  initTime = std::chrono::steady_clock::now();

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
  window(),
  mainLoop()
{

}
