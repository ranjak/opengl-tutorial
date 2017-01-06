#include "mainloop.hpp"
#include "log.hpp"
#include "system.hpp"
#include "orientation/interpolation.hpp"
#include "glfw/windowglfw.hpp"
#include <algorithm>


MainLoop *MainLoop::instance = nullptr;

using namespace std::chrono_literals;

const ogl::time MainLoop::TIMESTEP(10ms);

void MainLoop::requestExit()
{
  instance->setExit();
}

MainLoop::MainLoop() :
  mMainWindow(),
  mTutorial(),
  mExitRequested(false),
  mDoLogStats(false),
  mMaxFrameTime(0),
  mAccuFrameTimes(0),
  mNumFrames(0)
{
  if (instance)
    Log::error("MainLoop: Cannot create a new instance (there can be only one).");
  else
    instance = this;
}

bool MainLoop::init(int width, int height, const std::string& title)
{
  if (!System::init()) {
    return false;
  }

  mMainWindow.reset(new WindowGLFW(width, height, title));
  if (!mMainWindow) {
    Log::error("MainLoop: Could not create main window");
    return false;
  }

  mMainWindow->setCloseCallback([](Window*) { MainLoop::requestExit(); });
  mMainWindow->setSwapInterval(1);

  mTutorial.reset(new Interpolation(mMainWindow.get()));
  mTutorial->init();

  return true;
}

void MainLoop::run()
{
  // Simulated game time. Increases by a fixed amount at every game update.
  ogl::time gameTime(0);
  // Time at which the main loop was started.
  ogl::time startTime = System::now();

  while (!mExitRequested) {

    ogl::time realTimeElasped = System::now() - startTime;

    while (realTimeElasped > gameTime && !mExitRequested) {

      System::pollEvents();

      if (mExitRequested)
        break;

      mTutorial->update(TIMESTEP);

      gameTime += TIMESTEP;
    }

    if (mExitRequested)
        break;

    mTutorial->render();

    // Framerate statistics
    if (mDoLogStats) {
      ogl::time frameTime = System::now() - realTimeElasped - startTime;
      updateStats(frameTime);

      if (mAccuFrameTimes >= 500ms)
        logStats();
    }
  }
}

void MainLoop::setExit()
{
  mExitRequested = true;
}

void MainLoop::updateStats(ogl::time frameTime)
{
  mMaxFrameTime = std::max(mMaxFrameTime, frameTime);
  mAccuFrameTimes += frameTime;
  mNumFrames++;
}

void MainLoop::logStats()
{
  double avgFrame = mAccuFrameTimes.count() / static_cast<double>(mNumFrames);
  double fps = ogl::time::period::den / avgFrame;

  rlzLog(Log::INFO, "FPS: " << fps << ", Avg frame: " << avgFrame << " us, Max frame: " << mMaxFrameTime.count() << " us");

  mMaxFrameTime = ogl::time::zero();
  mAccuFrameTimes = ogl::time::zero();
  mNumFrames = 0;
}

