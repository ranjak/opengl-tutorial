#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "mainloop.hpp"
#include "window.hpp"
#include <memory>
#include <chrono>

struct System
{

  std::unique_ptr<Window> window;
  MainLoop mainLoop;

  static System& instance() { return *sysInstance; }

  static bool init();

  static std::chrono::milliseconds now();

private:
  System();
  static std::unique_ptr<System> sysInstance;
  static std::chrono::time_point<std::chrono::steady_clock> initTime;
};

#endif // SYSTEM_HPP
