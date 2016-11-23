#include "renderer.hpp"
#include "window.hpp"

// Whether OpenGL extensions have already been loaded
//static bool initDone = false;

Renderer::Renderer(Window* window) :
  mWindow(window)
{

}

void Renderer::render()
{
  mWindow->makeContextCurrent();

  // Draw stuff

  mWindow->swapBuffers();
}
