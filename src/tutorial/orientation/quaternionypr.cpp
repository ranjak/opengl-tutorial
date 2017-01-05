#include "quaternionypr.hpp"
#include "shader.hpp"
#include "transform.hpp"
#include <glutil/MatrixStack.h>

namespace
{

float CalcFrustumScale(float fFovDeg)
{
  const float degToRad = 3.14159f * 2.0f / 360.0f;
  float fFovRad = fFovDeg * degToRad;
  return 1.0f / tan(fFovRad / 2.0f);
}

const float fFrustumScale = CalcFrustumScale(20.0f);

} // namespace

QuaternionYPR::QuaternionYPR(Window* window) :
  Tutorial(window),
  mProgram(0),
  mPositionAttrib(0),
  mColorAttrib(0),

  mModelToCameraMatrixUnif(0),
  mCameraToClipMatrixUnif(0),
  mBaseColorUnif(0),

  mCameraToClipMatrix(0.0f),
  mOrientation(),
  mRightMultiply(true),
  mShip("assets/orientation/Ship.xml")
{
  InitializeProgram();

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);

  mWindow->addKeyCallback(onKeyboard);
}

void QuaternionYPR::InitializeProgram()
{
  mProgram = ogl::makePorgram({
                                ogl::Shader(GL_VERTEX_SHADER, "shaders/orientation/PosColorLocalTransform.vert"),
                                ogl::Shader(GL_FRAGMENT_SHADER, "shaders/orientation/ColorMultUniform.frag")
                              });

  mPositionAttrib = glGetAttribLocation(mProgram, "position");
  mColorAttrib = glGetAttribLocation(mProgram, "color");

  mModelToCameraMatrixUnif = glGetUniformLocation(mProgram, "modelToCameraMatrix");
  mCameraToClipMatrixUnif = glGetUniformLocation(mProgram, "cameraToClipMatrix");
  mBaseColorUnif = glGetUniformLocation(mProgram, "baseColor");

  float fzNear = 1.0f; float fzFar = 600.0f;

  mCameraToClipMatrix[0].x = fFrustumScale;
  mCameraToClipMatrix[1].y = fFrustumScale;
  mCameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
  mCameraToClipMatrix[2].w = -1.0f;
  mCameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);

  glUseProgram(mProgram);
  glUniformMatrix4fv(mCameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(mCameraToClipMatrix));
  glUseProgram(0);
}

void QuaternionYPR::OffsetOrientation(const glm::vec3& _axis, float fAngDeg)
{
  float fAngRad = ogl::degToRadf(fAngDeg);

  glm::vec3 axis = glm::normalize(_axis);

  axis = axis * std::sin(fAngRad / 2.0f);
  float scalar = std::cos(fAngRad / 2.0f);

  glm::fquat offset(scalar, axis.x, axis.y, axis.z);

  if(mRightMultiply)
      mOrientation = mOrientation * offset;
  else
      mOrientation = offset * mOrientation;

  mOrientation = glm::normalize(mOrientation);
}

#define SMALL_ANGLE_INCREMENT 9.0f

void QuaternionYPR::onKeyboard(Window* win, int key, int, Window::KeyAction act, short)
{
  if (act == Window::KEYUP) {
    return;
  }

  QuaternionYPR* thisScene = static_cast<QuaternionYPR*>(win->getTutorial());

  switch (key)
  {
  case 'W': thisScene->OffsetOrientation({1.0f, 0.0f, 0.0f}, SMALL_ANGLE_INCREMENT); break;
  case 'S': thisScene->OffsetOrientation({1.0f, 0.0f, 0.0f}, -SMALL_ANGLE_INCREMENT); break;

  case 'A': thisScene->OffsetOrientation({0.0f, 1.0f, 0.0f}, SMALL_ANGLE_INCREMENT); break;
  case 'D': thisScene->OffsetOrientation({0.0f, 1.0f, 0.0f}, -SMALL_ANGLE_INCREMENT); break;

  case 'Q': thisScene->OffsetOrientation({0.0f, 0.0f, 1.0f}, SMALL_ANGLE_INCREMENT); break;
  case 'E': thisScene->OffsetOrientation({0.0f, 0.0f, 1.0f}, -SMALL_ANGLE_INCREMENT); break;

  case 32:
    thisScene->mRightMultiply = !thisScene->mRightMultiply;
    break;
  }
}

void QuaternionYPR::framebufferSizeChanged(int w, int h)
{
  mCameraToClipMatrix[0].x = fFrustumScale * (h / (float)w);
  mCameraToClipMatrix[1].y = fFrustumScale;

  glUseProgram(mProgram);
  glUniformMatrix4fv(mCameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(mCameraToClipMatrix));
  glUseProgram(0);

  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void QuaternionYPR::renderInternal()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutil::MatrixStack currMatrix;
  currMatrix.Translate(glm::vec3(0.0f, 0.0f, -200.0f));
  currMatrix.ApplyMatrix(glm::mat4_cast(mOrientation));

  glUseProgram(mProgram);
  currMatrix.Scale(3.0, 3.0, 3.0);
  currMatrix.RotateX(-90);
  //Set the base color for this object.
  glUniform4f(mBaseColorUnif, 1.0, 1.0, 1.0, 1.0);
  glUniformMatrix4fv(mModelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(currMatrix.Top()));

  mShip.Render("tint");

  glUseProgram(0);
}

