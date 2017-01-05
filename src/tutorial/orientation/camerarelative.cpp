#include "camerarelative.hpp"
#include "shader.hpp"
#include "transform.hpp"
#include <glutil/MatrixStack.h>

namespace
{

glm::mat4 CalcLookAtMatrix(const glm::vec3& cameraPt, const glm::vec3& lookPt, const glm::vec3& upPt)
{
  glm::vec3 lookDir = glm::normalize(lookPt - cameraPt);
  glm::vec3 upDir = glm::normalize(upPt);

  glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
  glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

  glm::mat4 rotMat(1.0f);
  rotMat[0] = glm::vec4(rightDir, 0.0f);
  rotMat[1] = glm::vec4(perpUpDir, 0.0f);
  rotMat[2] = glm::vec4(-lookDir, 0.0f);

  rotMat = glm::transpose(rotMat);

  glm::mat4 transMat(1.0f);
  transMat[3] = glm::vec4(-cameraPt, 1.0f);

  return rotMat * transMat;
}

float CalcFrustumScale(float fFovDeg)
{
  const float degToRad = 3.14159f * 2.0f / 360.0f;
  float fFovRad = fFovDeg * degToRad;
  return 1.0f / tan(fFovRad / 2.0f);
}

const float fFrustumScale = CalcFrustumScale(20.0f);

} // namespace


CameraRelative::CameraRelative(Window* window) :
  Tutorial(window),
  mProgram(0),
  mPositionAttrib(0),
  mColorAttrib(0),

  mModelToCameraMatrixUnif(0),
  mCameraToClipMatrixUnif(0),
  mBaseColorUnif(0),

  mCameraToClipMatrix(0.0f),
  mOrientation(1.0f, 0.0f, 0.0f, 0.0f),
  mSphereCamRelPos(90.0f, 0.0f, 66.0f),
  mCamTarget(0.0f, 10.0f, 0.0f),
  mOffsetType(MODEL_RELATIVE),

  mShip("assets/orientation/Ship.xml"),
  mPlaneMesh("assets/orientation/UnitPlane.xml")
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

void CameraRelative::InitializeProgram()
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

void CameraRelative::framebufferSizeChanged(int w, int h)
{
  mCameraToClipMatrix[0].x = fFrustumScale * (h / (float)w);
  mCameraToClipMatrix[1].y = fFrustumScale;

  glUseProgram(mProgram);
  glUniformMatrix4fv(mCameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(mCameraToClipMatrix));
  glUseProgram(0);

  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

#define SMALL_ANGLE_INCREMENT 9.0f

void CameraRelative::onKeyboard(Window* win, int key, int, Window::KeyAction act, short mods)
{
  if (act == Window::KEYUP) {
    return;
  }
  // Controls are more precise when holding Shift
  float multiplier = (mods & Window::MOD_SHIFT) ? 1.0f : 10.0f;

  CameraRelative* thisScene = static_cast<CameraRelative*>(win->getTutorial());

  glm::vec3& sphereCamPos = thisScene->mSphereCamRelPos;

  switch (key)
  {
  case 'W': thisScene->OffsetOrientation({1.0f, 0.0f, 0.0f}, SMALL_ANGLE_INCREMENT); break;
  case 'S': thisScene->OffsetOrientation({1.0f, 0.0f, 0.0f}, -SMALL_ANGLE_INCREMENT); break;

  case 'A': thisScene->OffsetOrientation({0.0f, 1.0f, 0.0f}, SMALL_ANGLE_INCREMENT); break;
  case 'D': thisScene->OffsetOrientation({0.0f, 1.0f, 0.0f}, -SMALL_ANGLE_INCREMENT); break;

  case 'Q': thisScene->OffsetOrientation({0.0f, 0.0f, 1.0f}, SMALL_ANGLE_INCREMENT); break;
  case 'E': thisScene->OffsetOrientation({0.0f, 0.0f, 1.0f}, -SMALL_ANGLE_INCREMENT); break;

  case 'I': sphereCamPos.y -= 1.125f * multiplier; break;
  case 'K': sphereCamPos.y += 1.125f * multiplier; break;
  case 'J': sphereCamPos.x -= 1.125f * multiplier; break;
  case 'L': sphereCamPos.x += 1.125f * multiplier; break;
  case 'O': sphereCamPos.z -= 0.5f * multiplier; break;
  case 'U': sphereCamPos.z += 0.5f * multiplier; break;

  case 32:
    thisScene->mOffsetType = (thisScene->mOffsetType + 1) % 3;
    break;
  }

  sphereCamPos.y = glm::clamp(sphereCamPos.y, -78.75f, 10.0f);
}

glm::vec3 CameraRelative::ResolveCamPosition()
{
  float phi = ogl::degToRadf(mSphereCamRelPos.x);
  float theta = ogl::degToRadf(mSphereCamRelPos.y + 90.0f);

  float fSinTheta = std::sin(theta);
  float fCosTheta = std::cos(theta);
  float fCosPhi = std::cos(phi);
  float fSinPhi = std::sin(phi);

  glm::vec3 dirToCamera(fSinTheta * fCosPhi, fCosTheta, fSinTheta * fSinPhi);
  return (dirToCamera * mSphereCamRelPos.z) + mCamTarget;
}

void CameraRelative::OffsetOrientation(const glm::vec3& _axis, float fAngDeg)
{
  float fAngRad = ogl::degToRadf(fAngDeg);

  glm::vec3 axis = glm::normalize(_axis);

  axis = axis * std::sin(fAngRad / 2.0f);
  float scalar = std::cos(fAngRad / 2.0f);

  glm::fquat offset(scalar, axis.x, axis.y, axis.z);

  switch (mOffsetType) {
  case MODEL_RELATIVE:
    mOrientation = mOrientation * offset;
    break;
  case WORLD_RELATIVE:
    mOrientation = offset * mOrientation;
    break;
  case CAMERA_RELATIVE:
  {
    const glm::vec3 &camPos = ResolveCamPosition();
    const glm::mat4 &camMat = CalcLookAtMatrix(camPos, mCamTarget, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::fquat viewQuat = glm::quat_cast(camMat);
    glm::fquat invViewQuat = glm::conjugate(viewQuat);

    const glm::fquat &worldQuat = (invViewQuat * offset * viewQuat);
    mOrientation = worldQuat * mOrientation;
    break;
  }
  }

  mOrientation = glm::normalize(mOrientation);
}

void CameraRelative::renderInternal()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutil::MatrixStack currMatrix;
  currMatrix.SetMatrix(CalcLookAtMatrix(ResolveCamPosition(), mCamTarget, {0.0f, 1.0f, 0.0f}));

  glUseProgram(mProgram);
  // Render the plane
  {
    glutil::PushStack push(currMatrix);
    currMatrix.Scale(100.0f, 1.0f, 100.0f);

    glUniform4f(mBaseColorUnif, 0.2f, 0.5f, 0.2f, 1.0f);
    glUniformMatrix4fv(mModelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(currMatrix.Top()));

    mPlaneMesh.Render();
  }

  // Render the ship
  currMatrix.Translate(mCamTarget);
  currMatrix.ApplyMatrix(glm::mat4_cast(mOrientation));
  currMatrix.RotateX(-90);

  //Set the base color for this object.
  glUniform4f(mBaseColorUnif, 1.0, 1.0, 1.0, 1.0);
  glUniformMatrix4fv(mModelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(currMatrix.Top()));

  mShip.Render("tint");

  glUseProgram(0);
}
