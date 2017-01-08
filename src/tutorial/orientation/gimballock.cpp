#include "gimballock.hpp"
#include "shader.hpp"
#include "Mesh.h"
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

namespace
{

float CalcFrustumScale(float fFovDeg)
{
  const float degToRad = 3.14159f * 2.0f / 360.0f;
  float fFovRad = fFovDeg * degToRad;
  return 1.0f / tan(fFovRad / 2.0f);
}

const float fFrustumScale = CalcFrustumScale(20.0f);

std::unique_ptr<Framework::Mesh> g_Gimbals[3] = {nullptr};
const char *g_strGimbalNames[3] =
{
  "assets/orientation/LargeGimbal.xml",
  "assets/orientation/MediumGimbal.xml",
  "assets/orientation/SmallGimbal.xml",
};

std::unique_ptr<Framework::Mesh> g_pObject;

} // namespace

GimbalLock::GimbalLock(Window* window) :
  Tutorial(window),
  mProgram(0),
  mPositionAttrib(0),
  mColorAttrib(0),

  mModelToCameraMatrixUnif(0),
  mCameraToClipMatrixUnif(0),
  mBaseColorUnif(0),
  mCameraToClipMatrix(0.0f),
  mDrawGimbals(true),
  mAngles()
{
  InitializeProgram();

  // Load global objects
  for(int iLoop = 0; iLoop < 3; iLoop++)
  {
    if (!g_Gimbals[iLoop]) {
      g_Gimbals[iLoop].reset(new Framework::Mesh(g_strGimbalNames[iLoop]));
    }
  }

  if (!g_pObject) {
    g_pObject.reset(new Framework::Mesh("assets/orientation/Ship.xml"));
  }

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);

  mWindow->addKeyCallback(onKeyboard);
}


void GimbalLock::InitializeProgram()
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

void GimbalLock::DrawGimbal(glutil::MatrixStack &currMatrix, GimbalAxis eAxis, glm::vec4 baseColor)
{
  if(!mDrawGimbals)
    return;

  glutil::PushStack pusher(currMatrix);

  switch(eAxis)
  {
  case GIMBAL_X_AXIS:
    break;
  case GIMBAL_Y_AXIS:
    currMatrix.RotateZ(90.0f);
    currMatrix.RotateX(90.0f);
    break;
  case GIMBAL_Z_AXIS:
    currMatrix.RotateY(90.0f);
    currMatrix.RotateX(90.0f);
    break;
  }

  glUseProgram(mProgram);
  //Set the base color for this object.
  glUniform4fv(mBaseColorUnif, 1, glm::value_ptr(baseColor));
  glUniformMatrix4fv(mModelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(currMatrix.Top()));

  g_Gimbals[eAxis]->Render();

  glUseProgram(0);
}

void GimbalLock::renderInternal()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutil::MatrixStack currMatrix;
  currMatrix.Translate(glm::vec3(0.0f, 0.0f, -200.0f));
  currMatrix.RotateX(mAngles.fAngleX);
  DrawGimbal(currMatrix, GIMBAL_X_AXIS, glm::vec4(0.4f, 0.4f, 1.0f, 1.0f));
  currMatrix.RotateY(mAngles.fAngleY);
  DrawGimbal(currMatrix, GIMBAL_Y_AXIS, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  currMatrix.RotateZ(mAngles.fAngleZ);
  DrawGimbal(currMatrix, GIMBAL_Z_AXIS, glm::vec4(1.0f, 0.3f, 0.3f, 1.0f));

  glUseProgram(mProgram);
  currMatrix.Scale(3.0, 3.0, 3.0);
  currMatrix.RotateX(-90);
  //Set the base color for this object.
  glUniform4f(mBaseColorUnif, 1.0, 1.0, 1.0, 1.0);
  glUniformMatrix4fv(mModelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(currMatrix.Top()));

  g_pObject->Render("tint");

  glUseProgram(0);
}

void GimbalLock::framebufferSizeChanged(int w, int h)
{
  mCameraToClipMatrix[0].x = fFrustumScale * (h / (float)w);
  mCameraToClipMatrix[1].y = fFrustumScale;

  glUseProgram(mProgram);
  glUniformMatrix4fv(mCameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(mCameraToClipMatrix));
  glUseProgram(0);

  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

#define SMALL_ANGLE_INCREMENT 9.0f

void GimbalLock::onKeyboard(Window* win, int key, int, Window::Action act, short /*mods*/)
{
  if (act == Window::RELEASE) {
    return;
  }

  GimbalLock* thisScene = static_cast<GimbalLock*>(win->getTutorial());

  switch (key)
  {
  case 'W': thisScene->mAngles.fAngleX += SMALL_ANGLE_INCREMENT; break;
  case 'S': thisScene->mAngles.fAngleX -= SMALL_ANGLE_INCREMENT; break;

  case 'A': thisScene->mAngles.fAngleY += SMALL_ANGLE_INCREMENT; break;
  case 'D': thisScene->mAngles.fAngleY -= SMALL_ANGLE_INCREMENT; break;

  case 'Q': thisScene->mAngles.fAngleZ += SMALL_ANGLE_INCREMENT; break;
  case 'E': thisScene->mAngles.fAngleZ -= SMALL_ANGLE_INCREMENT; break;

  case 32:
    thisScene->mDrawGimbals = !thisScene->mDrawGimbals;
    break;
  }
}
