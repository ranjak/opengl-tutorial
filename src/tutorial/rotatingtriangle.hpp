#ifndef ROTATINGTRIANGLE_HPP
#define ROTATINGTRIANGLE_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class RotatingTriangle : public Tutorial
{
public:
  void init() override;

  void update(ogl::seconds delta) override;

  void render() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
};

#endif // ROTATINGTRIANGLE_HPP
