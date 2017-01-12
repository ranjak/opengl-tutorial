#include "orennayar.hpp"
#include "shader.hpp"
#include "transform.hpp"
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

ProgramData OrenNayar::LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
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
  data.facetSlopesDeviationUnif = glGetUniformLocation(data.theProgram, "sigma");

  GLuint projectionBlock = glGetUniformBlockIndex(data.theProgram, "Projection");
  glUniformBlockBinding(data.theProgram, projectionBlock, mProjectionBlockIndex);

  return data;
}

OrenNayar::OrenNayar(Window* window) :
  Tutorial(window),
  mProjectionBlockIndex(2),
  mProjectionUniformBuffer(0),
  mWhiteAmbDiffuseColor(LoadProgram("shaders/illumination/DirAmbVertexLighting_PN.vert", "shaders/illumination/ColorPassthrough.frag")),
  mVertexAmbDiffuseColor(LoadProgram("shaders/illumination/DirAmbVertexLighting_PCN.vert", "shaders/illumination/ColorPassthrough.frag")),
  mWhiteOrenNayar(LoadProgram("shaders/illumination/DirAmbVertexLighting_PN.vert", "shaders/illumination/ColorPassthrough.frag")),
  mVertexOrenNayar(LoadProgram("shaders/illumination/diramb_orennayar_pcn.vert", "shaders/illumination/ColorPassthrough.frag")),
  mCylinder("assets/illumination/UnitCylinder.xml"),
  mPlane("assets/illumination/LargePlane.xml"),
  mDrawColoredCyl(true),
  mUseOrenNayar(false),
  mViewPole(g_initialViewData, g_viewScale, glutil::MB_LEFT_BTN),
  mObjtPole(g_initialObjectData, 90.0f/250.0f, glutil::MB_RIGHT_BTN, &mViewPole),
  mLightIntensity(0.8f, 0.8f, 0.8f, 1.0f),
  mFacetSlopesDeviation(0.0f)
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
    OrenNayar* thisTut = static_cast<OrenNayar*>(win->getTutorial());
    thisTut->MouseMotion(static_cast<int>(x), static_cast<int>(y));
  });
  mWindow->setMouseButtonCallback([] (Window* win, int button, int state, int mods)
  {
    OrenNayar* thisTut = static_cast<OrenNayar*>(win->getTutorial());
    thisTut->MouseButton(button, state, mods);
  });
  mWindow->setScrollCallback([] (Window* win, double /*x*/, double y)
  {
    OrenNayar* thisTut = static_cast<OrenNayar*>(win->getTutorial());
    thisTut->MouseWheel(static_cast<int>(y));
  });
  mWindow->addKeyCallback([] (Window* win, int key, int, Window::Action act, short)
  {
    OrenNayar* thisTut = static_cast<OrenNayar*>(win->getTutorial());
    thisTut->onKeyboard(key, act);
  });
}

void OrenNayar::framebufferSizeChanged(int w, int h)
{
  glutil::MatrixStack persMatrix;
  persMatrix.Perspective(45.0f, (w / static_cast<float>(h)), g_fzNear, g_fzFar);

  glBindBuffer(GL_UNIFORM_BUFFER, mProjectionUniformBuffer);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(persMatrix.Top()));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glViewport(0, 0, w, h);
}

void OrenNayar::renderInternal()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutil::MatrixStack modelMatrix;
  modelMatrix.SetMatrix(mViewPole.CalcMatrix());

  glm::vec4 lightDirCameraSpace = modelMatrix.Top() * g_lightDirection;

  ProgramData& colorProgram = mUseOrenNayar ? mVertexOrenNayar : mVertexAmbDiffuseColor;
  ProgramData& whiteProgram = mUseOrenNayar ? mWhiteOrenNayar : mWhiteAmbDiffuseColor;

  glUseProgram(whiteProgram.theProgram);
  glUniform3fv(whiteProgram.dirToLightUnif, 1, glm::value_ptr(lightDirCameraSpace));
  glUniform4f(whiteProgram.ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
  glUniform4fv(whiteProgram.lightIntensityUnif, 1, glm::value_ptr(mLightIntensity));

  glUseProgram(colorProgram.theProgram);
  glUniform3fv(colorProgram.dirToLightUnif, 1, glm::value_ptr(lightDirCameraSpace));
  glUniform4f(colorProgram.ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
  glUniform4fv(colorProgram.lightIntensityUnif, 1, glm::value_ptr(mLightIntensity));

  if (mUseOrenNayar) {
    glUniform1f(colorProgram.facetSlopesDeviationUnif, mFacetSlopesDeviation);
//    glUseProgram(whiteProgram.theProgram);
//    glUniform1f(whiteProgram.facetSlopesDeviationUnif, mFacetSlopesDeviation);
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

void OrenNayar::MouseMotion(int x, int y)
{
  Framework::ForwardMouseMotion(mViewPole, x, y);
  Framework::ForwardMouseMotion(mObjtPole, x, y);
}

void OrenNayar::MouseButton(int button, int state, int mods)
{
  Framework::ForwardMouseButton(mViewPole, mWindow, button, state, mods);
  Framework::ForwardMouseButton(mObjtPole, mWindow, button, state, mods);
}

void OrenNayar::MouseWheel(int offset)
{
  Framework::ForwardMouseWheel(mViewPole, mWindow, offset);
  Framework::ForwardMouseWheel(mObjtPole, mWindow, offset);
}

void OrenNayar::onKeyboard(int key, Window::Action act)
{
  if (act == Window::RELEASE) {
    return;
  }

  switch (key) {
  case 32:
    mDrawColoredCyl = !mDrawColoredCyl;
    break;
  case 'O':
    mUseOrenNayar = !mUseOrenNayar;
    break;
  case 'L':
    if (mLightIntensity.r < 1.0f) {
      mLightIntensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
      mLightIntensity = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    }
    break;
  case 'B':
    mFacetSlopesDeviation -= ogl::PIf / 16.0f;
    break;
  case 'N':
    mFacetSlopesDeviation += ogl::PIf / 16.0f;
    break;
  default:
    break;
  }

  mFacetSlopesDeviation = glm::clamp(mFacetSlopesDeviation, 0.0f, ogl::PIf/2.0f);
}
