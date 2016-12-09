#ifndef ORTHOGRAPHICPRISM_HPP
#define ORTHOGRAPHICPRISM_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class OrthographicPrism : public Tutorial
{
public:
  void init() override;

  void render() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
};

#endif // ORTHOGRAPHICPRISM_HPP
