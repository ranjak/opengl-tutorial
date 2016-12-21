#ifndef WORLDSCENE_HPP
#define WORLDSCENE_HPP

#include "tutorial.hpp"
#include "Mesh.h"
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

  Framework::Mesh g_pConeMesh;
  Framework::Mesh g_pCylinderMesh;
  Framework::Mesh g_pCubeTintMesh;
  Framework::Mesh g_pCubeColorMesh;
  Framework::Mesh g_pPlaneMesh;
};

#endif // WORLDSCENE_HPP
