#ifndef WORLDSCENEOBJTOCAM_HPP
#define WORLDSCENEOBJTOCAM_HPP

#include "tutorial.hpp"
#include "Mesh.h"
#include "window.hpp"
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// Forward declarations
namespace glutil
{
class MatrixStack;
}

struct ProgramData
{
  GLuint theProgram;
  GLuint modelToClipMatrixUnif;
  GLuint baseColorUnif;
};


class WorldSceneObjToCam : public Tutorial
{
public:
  WorldSceneObjToCam(Window* window);

private:
  static void onKeyPressed(Window*, int, int, Window::Action, short);

  void renderInternal() override;
  void framebufferSizeChanged(int w, int h) override;

  void DrawTree(glutil::MatrixStack& modelMatrix, float fTrunkHeight = 2.0f, float fConeHeight = 3.0f);
  void DrawColumn(glutil::MatrixStack& modelMatrix, float fHeight = 5.0f);
  void DrawParthenon(glutil::MatrixStack& modelMatrix);
  void DrawForest(glutil::MatrixStack& modelMatrix);

  glm::vec3 ResolveCamPosition();

private:
  ProgramData mUniformColor;
  ProgramData mObjectColor;
  ProgramData mUniformColorTint;

  Framework::Mesh mConeMesh;
  Framework::Mesh mCylinderMesh;
  Framework::Mesh mCubeTintMesh;
  Framework::Mesh mCubeColorMesh;
  Framework::Mesh mPlaneMesh;

  bool mDrawLookatPoint = false;
  glm::vec3 mCamTarget;
  //In spherical coordinates.
  glm::vec3 mSphereCamRelPos;
  glm::mat4 mPerspectiveMatrix;
};

#endif // WORLDSCENEOBJTOCAM_HPP
