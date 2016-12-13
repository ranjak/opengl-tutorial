#ifndef MOVINGTRIANGLE_HPP
#define MOVINGTRIANGLE_HPP

#include "tutorial.hpp"
#include "system.hpp"
#include <glad/glad.h>

class MovingTriangleShader : public Tutorial
{
public:
  MovingTriangleShader(Window* win);

  void init() override;

  void update(ogl::seconds delta) override;

  void renderInternal() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
  GLint mTimeUniform;
  ogl::seconds mTime;
};

#endif // MOVINGTRIANGLE_HPP
