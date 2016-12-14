#ifndef INDEXEDDRAWING_HPP
#define INDEXEDDRAWING_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class IndexedDrawing : public Tutorial
{
public:
  IndexedDrawing(Window* window);

private:
  void renderInternal() override;

  void framebufferSizeChanged(int w, int h) override;

  void initShaders();
  void initObjects();

private:
  GLuint mProgram;
  GLuint mVao;
  GLuint mVao2;
  GLint mProjMatUniform;
  GLint mOffsetUniform;
};

#endif // INDEXEDDRAWING_HPP
