#ifndef MOVINGTRIANGLE_HPP
#define MOVINGTRIANGLE_HPP

#include "tutorial.hpp"
#include "system.hpp"
#include <glad/glad.h>

class MovingTriangleShader : public Tutorial
{
public:
  MovingTriangleShader();

  void init() override;

  void update(ogl::seconds delta) override;

  void render() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
  GLint mTimeUniform;
  ogl::seconds mTime;
};

#endif // MOVINGTRIANGLE_HPP
