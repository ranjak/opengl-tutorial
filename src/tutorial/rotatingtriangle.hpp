#ifndef ROTATINGTRIANGLE_HPP
#define ROTATINGTRIANGLE_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class RotatingTriangle : public Tutorial
{
public:
  RotatingTriangle(Window* win) : Tutorial(win) {}

  void init() override;

  void update(ogl::seconds delta) override;

  void renderInternal() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
};

#endif // ROTATINGTRIANGLE_HPP
