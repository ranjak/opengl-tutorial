#include "depthbuffering.hpp"
#include "util.hpp"

DepthBuffering::DepthBuffering(Window* window) :
  BaseVertex(window)
{
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);

  OGL_CHECK_ERROR();
}

void DepthBuffering::renderInternal()
{
  glClearDepth(1.0f);
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  OGL_CHECK_ERROR();

  BaseVertex::renderInternal();
}
