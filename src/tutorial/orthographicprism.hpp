#ifndef ORTHOGRAPHICPRISM_HPP
#define ORTHOGRAPHICPRISM_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class OrthographicPrism : public Tutorial
{
public:
  OrthographicPrism(Window* win);

  void renderInternal() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
  GLuint mVao;
};

#endif // ORTHOGRAPHICPRISM_HPP
