#ifndef MOVINGTRIANGLE_HPP
#define MOVINGTRIANGLE_HPP

#include "tutorial.hpp"
#include "system.hpp"
#include <glad/glad.h>

class MovingTriangle : public Tutorial
{
public:
  MovingTriangle();

  void init() override;

  void update(ogl::seconds delta) override;

  void render() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
  GLint mOffsetUniform;
  ogl::seconds mTime;
};

#endif // MOVINGTRIANGLE_HPP
