#ifndef TUTORIAL_HPP
#define TUTORIAL_HPP

#include "system.hpp"

class Window;

class Tutorial
{
public:
  Tutorial(Window* window);

  virtual ~Tutorial() = 0;

  virtual void init() {}

  virtual void update(ogl::seconds /*delta*/) {}

  void render();

  virtual void renderInternal() = 0;

  virtual void framebufferSizeChanged(int w, int h);

protected:
  Window* mWindow;
  unsigned int mVao;
};

inline Tutorial::~Tutorial() {}

#endif // TUTORIAL_HPP
