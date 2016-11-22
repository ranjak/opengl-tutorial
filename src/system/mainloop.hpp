#ifndef MAINLOOP_HPP
#define MAINLOOP_HPP

#include <cstdint>
#include <string>
#include <memory>
#include <chrono>


class MainLoop
{
public:
  static const std::chrono::milliseconds TIMESTEP;

  static void requestExit();

  MainLoop();
  MainLoop(const MainLoop &) = delete;
  MainLoop& operator=(const MainLoop &) = delete;

  void run();

  void setExit();

private:
  void updateStats(std::chrono::milliseconds frameTime);
  void logStats();

private:
  static MainLoop *instance;

  bool mExitRequested;

  std::chrono::milliseconds mMaxFrameTime;
  std::chrono::milliseconds mAccuFrameTimes;
  int mNumFrameTimes;
};


#endif // MAINLOOP_HPP
