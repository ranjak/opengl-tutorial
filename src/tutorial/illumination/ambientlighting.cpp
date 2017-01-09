#include "ambientlighting.hpp"
#include "shader.hpp"
#include "framework/MousePole.h"
#include <glutil/MatrixStack.h>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{

// View/Object Setup
glutil::ViewData g_initialViewData =
{
  glm::vec3(0.0f, 0.5f, 0.0f),
  glm::fquat(0.92387953f, 0.3826834f, 0.0f, 0.0f),
  5.0f,
  0.0f
};

glutil::ViewScale g_viewScale =
{
  3.0f, 20.0f,
  1.5f, 0.5f,
  0.0f, 0.0f,		//No camera movement.
  90.0f/250.0f
};

glutil::ObjectData g_initialObjectData =
{
  glm::vec3(0.0f, 0.5f, 0.0f),
  glm::fquat(1.0f, 0.0f, 0.0f, 0.0f),
};

glm::vec4 g_lightDirection(0.866f, 0.5f, 0.0f, 0.0f);

float g_fzNear = 1.0f;
float g_fzFar = 1000.0f;

} // namespace

ProgramData AmbientLighting::LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
  ProgramData data;
  data.theProgram = ogl::makePorgram({
                                       ogl::Shader(GL_VERTEX_SHADER, strVertexShader),
                                       ogl::Shader(GL_FRAGMENT_SHADER, strFragmentShader)
                                     });
  data.modelToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "modelToCameraMatrix");
  data.normalModelToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "normalModelToCameraMatrix");
  data.dirToLightUnif = glGetUniformLocation(data.theProgram, "dirToLight");
  data.lightIntensityUnif = glGetUniformLocation(data.theProgram, "lightIntensity");
  data.ambientIntensityUnif = glGetUniformLocation(data.theProgram, "ambientIntensity");

  GLuint projectionBlock = glGetUniformBlockIndex(data.theProgram, "Projection");
  glUniformBlockBinding(data.theProgram, projectionBlock, mProjectionBlockIndex);

  return data;
}

AmbientLighting::AmbientLighting(Window* window) :
  Tutorial(window),
  mProjectionBlockIndex(2),
  mProjectionUniformBuffer(0),
  mWhiteDiffuseColor(LoadProgram("shaders/illumination/DirVertexLighting_PN.vert", "shaders/illumination/ColorPassthrough.frag")),
  mVertexDiffuseColor(LoadProgram("shaders/illumination/DirVertexLighting_PCN.vert", "shaders/illumination/ColorPassthrough.frag")),
  mWhiteAmbDiffuseColor(LoadProgram("shaders/illumination/DirAmbVertexLighting_PN.vert", "shaders/illumination/ColorPassthrough.frag")),
  mVertexAmbDiffuseColor(LoadProgram("shaders/illumination/DirAmbVertexLighting_PCN.vert", "shaders/illumination/ColorPassthrough.frag")),
  mCylinder("assets/illumination/UnitCylinder.xml"),
  mPlane("assets/illumination/LargePlane.xml"),
  mDrawColoredCyl(true),
  mUseAmbientLight(true),
  mViewPole(g_initialViewData, g_viewScale, glutil::MB_LEFT_BTN),
  mObjtPole(g_initialObjectData, 90.0f/250.0f, glutil::MB_RIGHT_BTN, &mViewPole)
{
  glGenBuffers(1, &mProjectionUniformBuffer);
  glBindBuffer(GL_UNIFORM_BUFFER, mProjectionUniformBuffer);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER, mProjectionBlockIndex, mProjectionUniformBuffer, 0, sizeof(glm::mat4));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);

  mWindow->setMouseMovementCallback([] (Window* win, double x, double y)
  {
    AmbientLighting* thisTut = static_cast<AmbientLighting*>(win->getTutorial());
    thisTut->MouseMotion(static_cast<int>(x), static_cast<int>(y));
  });
  mWindow->setMouseButtonCallback([] (Window* win, int button, int state, int mods)
  {
    AmbientLighting* thisTut = static_cast<AmbientLighting*>(win->getTutorial());
    thisTut->MouseButton(button, state, mods);
  });
  mWindow->setScrollCallback([] (Window* win, double /*x*/, double y)
  {
    AmbientLighting* thisTut = static_cast<AmbientLighting*>(win->getTutorial());
    thisTut->MouseWheel(static_cast<int>(y));
  });
  mWindow->addKeyCallback([] (Window* win, int key, int, Window::Action act, short)
  {
    AmbientLighting* thisTut = static_cast<AmbientLighting*>(win->getTutorial());
    thisTut->onKeyboard(key, act);
  });
}

void AmbientLighting::framebufferSizeChanged(int w, int h)
{
  glutil::MatrixStack persMatrix;
  persMatrix.Perspective(45.0f, (w / static_cast<float>(h)), g_fzNear, g_fzFar);

  glBindBuffer(GL_UNIFORM_BUFFER, mProjectionUniformBuffer);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(persMatrix.Top()));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glViewport(0, 0, w, h);
}

void AmbientLighting::renderInternal()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutil::MatrixStack modelMatrix;
  modelMatrix.SetMatrix(mViewPole.CalcMatrix());

  glm::vec4 lightDirCameraSpace = modelMatrix.Top() * g_lightDirection;

  ProgramData& colorProgram = mUseAmbientLight ? mVertexAmbDiffuseColor : mVertexDiffuseColor;
  ProgramData& whiteProgram = mUseAmbientLight ? mWhiteAmbDiffuseColor : mWhiteDiffuseColor;

  glUseProgram(whiteProgram.theProgram);
  glUniform3fv(whiteProgram.dirToLightUnif, 1, glm::value_ptr(lightDirCameraSpace));
  glUseProgram(colorProgram.theProgram);
  glUniform3fv(colorProgram.dirToLightUnif, 1, glm::value_ptr(lightDirCameraSpace));

  if (mUseAmbientLight) {
    glUniform4f(colorProgram.ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
    glUniform4f(colorProgram.lightIntensityUnif, 0.8f, 0.8f, 0.8f, 1.0f);
    glUseProgram(whiteProgram.theProgram);
    glUniform4f(whiteProgram.ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
    glUniform4f(whiteProgram.lightIntensityUnif, 0.8f, 0.8f, 0.8f, 1.0f);
  }
  else {
    glUniform4f(colorProgram.lightIntensityUnif, 1.0f, 1.0f, 1.0f, 1.0f);
    glUseProgram(whiteProgram.theProgram);
    glUniform4f(whiteProgram.lightIntensityUnif, 1.0f, 1.0f, 1.0f, 1.0f);
  }
  glUseProgram(0);

  {
    glutil::PushStack push(modelMatrix);

    //Render the ground plane.
    {
      glutil::PushStack push(modelMatrix);

      glUseProgram(whiteProgram.theProgram);
      glUniformMatrix4fv(whiteProgram.modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
      glm::mat3 normMatrix(modelMatrix.Top());
      glUniformMatrix3fv(whiteProgram.normalModelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(normMatrix));
      mPlane.Render();
      glUseProgram(0);
    }

    //Render the Cylinder
    {
      glutil::PushStack push(modelMatrix);

      modelMatrix.ApplyMatrix(mObjtPole.CalcMatrix());

      if(mDrawColoredCyl)
      {
        glUseProgram(colorProgram.theProgram);
        glUniformMatrix4fv(colorProgram.modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
        glm::mat3 normMatrix(modelMatrix.Top());
        glUniformMatrix3fv(colorProgram.normalModelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(normMatrix));
        mCylinder.Render("lit-color");
      }
      else
      {
        glUseProgram(whiteProgram.theProgram);
        glUniformMatrix4fv(whiteProgram.modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
        glm::mat3 normMatrix(modelMatrix.Top());
        glUniformMatrix3fv(whiteProgram.normalModelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(normMatrix));
        mCylinder.Render("lit");
      }
      glUseProgram(0);
    }
  }
}

void AmbientLighting::MouseMotion(int x, int y)
{
  Framework::ForwardMouseMotion(mViewPole, x, y);
  Framework::ForwardMouseMotion(mObjtPole, x, y);
}

void AmbientLighting::MouseButton(int button, int state, int mods)
{
  Framework::ForwardMouseButton(mViewPole, mWindow, button, state, mods);
  Framework::ForwardMouseButton(mObjtPole, mWindow, button, state, mods);
}

void AmbientLighting::MouseWheel(int offset)
{
  Framework::ForwardMouseWheel(mViewPole, mWindow, offset);
  Framework::ForwardMouseWheel(mObjtPole, mWindow, offset);
}

void AmbientLighting::onKeyboard(int key, Window::Action act)
{
  if (act == Window::RELEASE) {
    return;
  }

  switch (key) {
  case 32:
    mDrawColoredCyl = !mDrawColoredCyl;
    break;
  case 'A':
    mUseAmbientLight = !mUseAmbientLight;
    break;
  default:
    break;
  }
}
