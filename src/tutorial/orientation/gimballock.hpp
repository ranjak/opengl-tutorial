#ifndef GIMBALLOCK_HPP
#define GIMBALLOCK_HPP

#include "tutorial.hpp"
#include "window.hpp"
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glutil/MatrixStack.h>

enum GimbalAxis
{
  GIMBAL_X_AXIS,
  GIMBAL_Y_AXIS,
  GIMBAL_Z_AXIS,
};

struct GimbalAngles
{
  GimbalAngles()
    : fAngleX(0.0f)
    , fAngleY(0.0f)
    , fAngleZ(0.0f)
  {}

  float fAngleX;
  float fAngleY;
  float fAngleZ;
};

class GimbalLock : public Tutorial
{
public:
  GimbalLock(Window* window);

private:
  static void onKeyboard(Window* win, int key, int, Window::KeyAction act, short /*mods*/);

  void framebufferSizeChanged(int w, int h) override;
  void renderInternal() override;

  void InitializeProgram();
  void DrawGimbal(glutil::MatrixStack& currMatrix, GimbalAxis eAxis, glm::vec4 baseColor);

private:
  GLuint mProgram;
  GLuint mPositionAttrib;
  GLuint mColorAttrib;

  GLuint mModelToCameraMatrixUnif;
  GLuint mCameraToClipMatrixUnif;
  GLuint mBaseColorUnif;

  glm::mat4 mCameraToClipMatrix;

  bool mDrawGimbals;
  GimbalAngles mAngles;
};

#endif // GIMBALLOCK_HPP
