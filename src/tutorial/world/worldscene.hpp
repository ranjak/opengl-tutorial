#ifndef WORLDSCENE_HPP
#define WORLDSCENE_HPP

#include "tutorial.hpp"
#include "Mesh.h"
#include <glad/glad.h>
#include <glm/vec3.hpp>

// Forward declarations
namespace glutil
{
class MatrixStack;
}

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

  void DrawTree(glutil::MatrixStack& modelMatrix, float fTrunkHeight = 2.0f, float fConeHeight = 3.0f);
  void DrawColumn(glutil::MatrixStack& modelMatrix, float fHeight = 5.0f);
  void DrawParthenon(glutil::MatrixStack& modelMatrix);
  void DrawForest(glutil::MatrixStack& modelMatrix);

  glm::vec3 ResolveCamPosition();

private:
  ProgramData UniformColor;
  ProgramData ObjectColor;
  ProgramData UniformColorTint;

  Framework::Mesh g_pConeMesh;
  Framework::Mesh g_pCylinderMesh;
  Framework::Mesh g_pCubeTintMesh;
  Framework::Mesh g_pCubeColorMesh;
  Framework::Mesh g_pPlaneMesh;

  bool g_bDrawLookatPoint = false;
  glm::vec3 g_camTarget;
  //In spherical coordinates.
  glm::vec3 g_sphereCamRelPos;
};

#endif // WORLDSCENE_HPP
