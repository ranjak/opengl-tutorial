#ifndef MANUALPERSPECTIVE_HPP
#define MANUALPERSPECTIVE_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class ManualPerspective : public Tutorial
{
public:
public:
  void init() override;

  void render() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
};

#endif // MANUALPERSPECTIVE_HPP
