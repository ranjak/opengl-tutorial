#include "mainloop.hpp"
#include "log.hpp"
#include "system.hpp"
#include "glfw/windowglfw.hpp"
#include <algorithm>


MainLoop *MainLoop::instance = nullptr;

const ogl::time MainLoop::TIMESTEP(10);

void MainLoop::requestExit()
{
  instance->setExit();
}

MainLoop::MainLoop() :
  mMainWindow(),
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

MainLoop::~MainLoop()
{
  System::cleanup();
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

  return true;
}

void MainLoop::run()
{
  using namespace std::chrono_literals;

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

      gameTime += TIMESTEP;
    }

    if (mExitRequested)
        break;

   // mDisplay.render(mGame);

    // Framerate statistics
    ogl::time frameTime = System::now() - realTimeElasped - startTime;
    updateStats(frameTime);

    if (mAccuFrameTimes >= 500ms)
      logStats();
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

