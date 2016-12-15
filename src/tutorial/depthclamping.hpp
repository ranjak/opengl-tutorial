#ifndef DEPTHCLAMPING_HPP
#define DEPTHCLAMPING_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class DepthClamping : public Tutorial
{
public:
  DepthClamping(Window* window);

private:
  void renderInternal() override;
  void framebufferSizeChanged(int w, int h) override;

  void initShaders();
  void initObjects();

private:
  GLuint mProgram;
  GLuint mVao;
  GLint mProjMatUniform;
  GLint mOffsetUniform;
};

#endif // DEPTHCLAMPING_HPP
