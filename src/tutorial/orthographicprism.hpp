#ifndef ORTHOGRAPHICPRISM_HPP
#define ORTHOGRAPHICPRISM_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class OrthographicPrism : public Tutorial
{
public:
  OrthographicPrism(Window* win) : Tutorial(win) {}

  void init() override;

  void renderInternal() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
};

#endif // ORTHOGRAPHICPRISM_HPP
