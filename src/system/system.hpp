#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <chrono>

namespace ogl
{
using time = std::chrono::microseconds;

using seconds = std::chrono::duration<float, std::chrono::seconds::period>;
}

struct System
{
  ~System();

  static bool init();

  static ogl::time now();

  static void pollEvents();

  static void cleanup();

private:
  System();

  static std::chrono::time_point<std::chrono::steady_clock> initTime;
};

#endif // SYSTEM_HPP
