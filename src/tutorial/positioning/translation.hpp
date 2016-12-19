#ifndef TRANSLATION_HPP
#define TRANSLATION_HPP

#include "tutorial.hpp"
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <vector>

// Represents an individual object (actually only provides a translation matrix)
struct Instance
{
  typedef glm::mat4(*OffsetFunc)(float);

  Instance(OffsetFunc f) : CalcOffset(f) {}

  OffsetFunc CalcOffset;

  glm::mat4 ConstructMatrix(float fElapsedTime);
};

class Translation : public Tutorial
{
public:
  Translation(Window* window);

private:
  void renderInternal() override;
  void framebufferSizeChanged(int w, int h) override;

private:
  // GL objects
  GLuint mProgram;
  GLuint mVao;

  // Shader uniforms
  GLint mModelViewMatUnif;
  GLint mProjMatUnif;

  // Object position list
  std::vector<Instance> mPositions;
};

#endif // TRANSLATION_HPP
