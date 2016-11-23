#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <chrono>

namespace ogl
{
using time = std::chrono::microseconds;
}

struct System
{
  static bool init();

  static ogl::time now();

  static void pollEvents();

  static void cleanup();

private:
  static bool wasInit;
  static std::chrono::time_point<std::chrono::steady_clock> initTime;
};

#endif // SYSTEM_HPP
