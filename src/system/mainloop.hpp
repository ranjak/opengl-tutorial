#ifndef MAINLOOP_HPP
#define MAINLOOP_HPP

#include "window.hpp"
#include "system.hpp"
#include "tutorial.hpp"
#include <string>
#include <memory>


class MainLoop
{
public:
  static const ogl::time TIMESTEP;

  static void requestExit();

  MainLoop();
  MainLoop(const MainLoop &) = delete;
  MainLoop& operator=(const MainLoop &) = delete;
  ~MainLoop();

  bool init(int width=640, int height=480, const std::string& title="OpenGL Window");

  void run();

  void setExit();

private:
  void updateStats(ogl::time frameTime);
  void logStats();

private:
  static MainLoop *instance;

  std::unique_ptr<Window> mMainWindow;
  std::unique_ptr<Tutorial> mTutorial;
  bool mExitRequested;

  bool mDoLogStats;
  ogl::time mMaxFrameTime;
  ogl::time mAccuFrameTimes;
  int mNumFrames;
};


#endif // MAINLOOP_HPP
