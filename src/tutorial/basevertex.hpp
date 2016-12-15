#ifndef BASEVERTEX_HPP
#define BASEVERTEX_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class BaseVertex : public Tutorial
{
public:
  BaseVertex(Window* window);

protected:
  void renderInternal() override;

private:
  void framebufferSizeChanged(int w, int h) override;

  void initShaders();
  void initObjects();

private:
  GLuint mProgram;
  GLuint mVao;
  GLint mProjMatUniform;
  GLint mOffsetUniform;
};

#endif // BASEVERTEX_HPP
