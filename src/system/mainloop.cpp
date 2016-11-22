#include "mainloop.hpp"
#include "log.hpp"
#include "system.hpp"
#include <algorithm>


MainLoop *MainLoop::instance = nullptr;

const std::chrono::milliseconds MainLoop::TIMESTEP(10);

void MainLoop::requestExit()
{
  instance->setExit();
}

MainLoop::MainLoop() :
  mExitRequested(false),
  mMaxFrameTime(0),
  mAccuFrameTimes(0),
  mNumFrameTimes(0)
{
  if (instance)
    Log::error("MainLoop: Cannot create a new instance (there can be only one).");
  else
    instance = this;
}

void MainLoop::run()
{
  using namespace std::chrono_literals;

  // Simulated game time. Increases by a fixed amount at every game update.
  std::chrono::milliseconds gameTime(0);
  // Time at which the main loop was started.
  std::chrono::milliseconds startTime = System::now();

  while (!mExitRequested) {

    std::chrono::milliseconds realTimeElasped = System::now() - startTime;

    while (realTimeElasped > gameTime && !mExitRequested) {

      //mInput->handle();

      if (mExitRequested)
        break;

      gameTime += TIMESTEP;
    }

    if (mExitRequested)
        break;

   // mDisplay.render(mGame);

    // Framerate statistics
    std::chrono::milliseconds frameTime = System::now() - realTimeElasped - startTime;
    updateStats(frameTime);

    if (mAccuFrameTimes >= 500ms)
      logStats();
  }
}

void MainLoop::setExit()
{
  mExitRequested = true;
}

void MainLoop::updateStats(std::chrono::milliseconds frameTime)
{
  mMaxFrameTime = std::max(mMaxFrameTime, frameTime);
  mAccuFrameTimes += frameTime;
  mNumFrameTimes++;
}

void MainLoop::logStats()
{
  using namespace std::chrono_literals;

  double avgFrame = mAccuFrameTimes.count() / static_cast<double>(mNumFrameTimes);
  double fps = 1000.0 / avgFrame;

  rlzLog(Log::INFO, "FPS: " << fps << ", Avg frame: " << avgFrame << " ms, Max frame: " << mMaxFrameTime.count() << " ms");

  mMaxFrameTime = 0ms;
  mAccuFrameTimes = 0ms;
  mNumFrameTimes = 0;
}

