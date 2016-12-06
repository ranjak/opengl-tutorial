#ifndef ROTATINGTRIANGLE_HPP
#define ROTATINGTRIANGLE_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class RotatingTriangle : public Tutorial
{
public:
  void init();

  void update();

  void render();

private:
  GLuint mProgram;
  GLuint mBuffer;
};

#endif // ROTATINGTRIANGLE_HPP
