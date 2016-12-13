#ifndef MANUALPERSPECTIVE_HPP
#define MANUALPERSPECTIVE_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

class ManualPerspective : public Tutorial
{
public:
  ManualPerspective(Window* win) : Tutorial(win) {}

  void init() override;

  void renderInternal() override;

private:
  GLuint mProgram;
  GLuint mBuffer;
};

#endif // MANUALPERSPECTIVE_HPP
