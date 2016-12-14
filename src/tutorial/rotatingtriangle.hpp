#ifndef ROTATINGTRIANGLE_HPP
#define ROTATINGTRIANGLE_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class RotatingTriangle : public Tutorial
{
public:
  RotatingTriangle(Window* win);

  void update(ogl::seconds delta) override;

  void renderInternal() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
  GLuint mVao;
};

#endif // ROTATINGTRIANGLE_HPP
