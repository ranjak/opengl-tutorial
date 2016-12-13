#ifndef MATRIXPERSPECTIVE_HPP
#define MATRIXPERSPECTIVE_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class MatrixPerspective : public Tutorial
{
public:
  MatrixPerspective(Window* win) : Tutorial(win) {}

  void init() override;

  void renderInternal() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
};

#endif // MATRIXPERSPECTIVE_HPP
