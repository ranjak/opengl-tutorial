#ifndef QUATERNIONYPR_HPP
#define QUATERNIONYPR_HPP

#include "tutorial.hpp"
#include "window.hpp"
#include "Mesh.h"
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

class QuaternionYPR : public Tutorial
{
public:
  QuaternionYPR(Window* window);

private:
  static void onKeyboard(Window* win, int key, int, Window::Action act, short /*mods*/);

  void framebufferSizeChanged(int w, int h) override;
  void renderInternal() override;

  void InitializeProgram();
  void OffsetOrientation(const glm::vec3& _axis, float fAngDeg);

private:
  GLuint mProgram;
  GLuint mPositionAttrib;
  GLuint mColorAttrib;

  GLuint mModelToCameraMatrixUnif;
  GLuint mCameraToClipMatrixUnif;
  GLuint mBaseColorUnif;

  glm::mat4 mCameraToClipMatrix;
  glm::fquat mOrientation;
  bool mRightMultiply;

  Framework::Mesh mShip;
};

#endif // QUATERNIONYPR_HPP
