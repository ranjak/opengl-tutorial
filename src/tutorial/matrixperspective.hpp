#ifndef MATRIXPERSPECTIVE_HPP
#define MATRIXPERSPECTIVE_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class MatrixPerspective : public Tutorial
{
public:
  MatrixPerspective(Window* win);

private:
  void renderInternal() override;

  void framebufferSizeChanged(int w, int h) override;

private:
  GLuint mProgram;
  GLuint mBuffer;
  GLuint mVao;
  GLint mProjMatUniform;
};

#endif // MATRIXPERSPECTIVE_HPP
