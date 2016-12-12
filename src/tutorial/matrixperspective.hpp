#ifndef MATRIXPERSPECTIVE_HPP
#define MATRIXPERSPECTIVE_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class MatrixPerspective : public Tutorial
{
public:
public:
  void init() override;

  void render() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
};

#endif // MATRIXPERSPECTIVE_HPP
