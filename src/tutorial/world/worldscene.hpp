#ifndef WORLDSCENE_HPP
#define WORLDSCENE_HPP

#include "tutorial.hpp"
#include <glad/glad.h>

struct ProgramData
{
  GLuint theProgram;
  GLuint modelToWorldMatrixUnif;
  GLuint worldToCameraMatrixUnif;
  GLuint cameraToClipMatrixUnif;
  GLuint baseColorUnif;
};

class WorldScene : public Tutorial
{
public:
  WorldScene(Window* window);

private:
  void renderInternal() override;
  void framebufferSizeChanged(int w, int h) override;

private:
  ProgramData UniformColor;
  ProgramData ObjectColor;
  ProgramData UniformColorTint;
};

#endif // WORLDSCENE_HPP
