#ifndef CAMERARELATIVE_HPP
#define CAMERARELATIVE_HPP

#include "tutorial.hpp"
#include "window.hpp"
#include "Mesh.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad/glad.h>

class CameraRelative : public Tutorial
{
public:
  CameraRelative(Window* window);

private:
  static void onKeyboard(Window* win, int key, int, Window::Action act, short mods);

  void framebufferSizeChanged(int w, int h) override;
  void renderInternal() override;

  void InitializeProgram();
  void OffsetOrientation(const glm::vec3& _axis, float fAngDeg);
  glm::vec3 ResolveCamPosition();

private:
  enum OffsetType { MODEL_RELATIVE, WORLD_RELATIVE, CAMERA_RELATIVE };

  GLuint mProgram;
  GLuint mPositionAttrib;
  GLuint mColorAttrib;

  GLuint mModelToCameraMatrixUnif;
  GLuint mCameraToClipMatrixUnif;
  GLuint mBaseColorUnif;

  glm::mat4 mCameraToClipMatrix;
  glm::fquat mOrientation;
  glm::vec3 mSphereCamRelPos;
  glm::vec3 mCamTarget;
  int mOffsetType;

  Framework::Mesh mShip;
  Framework::Mesh mPlaneMesh;
};

#endif // CAMERARELATIVE_HPP
