#include "fragmentattenuation.hpp"
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
#include <cmath>

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

float g_fzNear = 1.0f;
float g_fzFar = 1000.0f;

struct UnProjectionBlock
{
  glm::mat4 clipToCameraMatrix;
  glm::ivec2 windowSize;
};

} // namespace

FragmentAttenuation::FragmentAttenuation(Window* window) :
  Tutorial(window),
  mProjectionBlockIndex(2),
  mUnprojectionBlockIndex(3),
  mProjectionUniformBuffer(0),
  mUnprojectionUniformBuffer(0),

  mWhiteFragmentDiffuse(LoadProgram("shaders/illumination/fragment/FragLightAtten_PN.vert", "shaders/illumination/fragment/FragLightAtten.frag")),
  mColorFragmentDiffuse(LoadProgram("shaders/illumination/fragment/FragLightAtten_PCN.vert", "shaders/illumination/fragment/FragLightAtten.frag")),
  mUnlit(LoadUnlitProgram("shaders/illumination/PosTransform.vert", "shaders/illumination/UniformColor.frag")),

  mCylinder("assets/illumination/UnitCylinder.xml"),
  mPlane("assets/illumination/LargePlane.xml"),
  mCube("assets/illumination/UnitCube.xml"),
  mDrawColoredCyl(true),
  mDrawLight(true),
  mScaleCylinder(false),
  mUseRSquare(false),
  mViewPole(g_initialViewData, g_viewScale, glutil::MB_LEFT_BTN),
  mObjtPole(g_initialObjectData, 90.0f/250.0f, glutil::MB_RIGHT_BTN, &mViewPole),

  mLightHeight(1.5f),
  mLightRadius(1.0f),
  mLightTimer(Framework::Timer::TT_LOOP, 5.0f),
  mLightAttenuation(1.0f)
{
  glGenBuffers(1, &mProjectionUniformBuffer);
  glBindBuffer(GL_UNIFORM_BUFFER, mProjectionUniformBuffer);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER, mProjectionBlockIndex, mProjectionUniformBuffer, 0, sizeof(glm::mat4));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glGenBuffers(1, &mUnprojectionUniformBuffer);
  glBindBuffer(GL_UNIFORM_BUFFER, mUnprojectionUniformBuffer);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(UnProjectionBlock), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER, mUnprojectionBlockIndex, mUnprojectionUniformBuffer, 0, sizeof(UnProjectionBlock));
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
    FragmentAttenuation* thisTut = static_cast<FragmentAttenuation*>(win->getTutorial());
    thisTut->MouseMotion(static_cast<int>(x), static_cast<int>(y));
  });
  mWindow->setMouseButtonCallback([] (Window* win, int button, int state, int mods)
  {
    FragmentAttenuation* thisTut = static_cast<FragmentAttenuation*>(win->getTutorial());
    thisTut->MouseButton(button, state, mods);
  });
  mWindow->setScrollCallback([] (Window* win, double /*x*/, double y)
  {
    FragmentAttenuation* thisTut = static_cast<FragmentAttenuation*>(win->getTutorial());
    thisTut->MouseWheel(static_cast<int>(y));
  });
  mWindow->addKeyCallback([] (Window* win, int key, int, Window::Action act, short mods)
  {
    FragmentAttenuation* thisTut = static_cast<FragmentAttenuation*>(win->getTutorial());
    thisTut->onKeyboard(key, act, mods);
  });
}

ProgramData FragmentAttenuation::LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
  ProgramData data;
  data.theProgram = ogl::makePorgram({
                                       ogl::Shader(GL_VERTEX_SHADER, strVertexShader),
                                       ogl::Shader(GL_FRAGMENT_SHADER, strFragmentShader)
                                     });
  data.modelToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "modelToCameraMatrix");
  data.lightIntensityUnif = glGetUniformLocation(data.theProgram, "lightIntensity");
  data.ambientIntensityUnif = glGetUniformLocation(data.theProgram, "ambientIntensity");

  data.normalModelToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "normalModelToCameraMatrix");
  data.cameraSpaceLightPosUnif = glGetUniformLocation(data.theProgram, "cameraSpaceLightPos");
  data.lightAttenuationUnif = glGetUniformLocation(data.theProgram, "lightAttenuation");
  data.bUseRSquareUnif = glGetUniformLocation(data.theProgram, "bUseRSquare");

  GLuint projectionBlock = glGetUniformBlockIndex(data.theProgram, "Projection");
  glUniformBlockBinding(data.theProgram, projectionBlock, mProjectionBlockIndex);
  GLuint unprojectionBlock = glGetUniformBlockIndex(data.theProgram, "UnProjection");
  glUniformBlockBinding(data.theProgram, unprojectionBlock, mUnprojectionBlockIndex);

  return data;
}

UnlitProgramData FragmentAttenuation::LoadUnlitProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
  UnlitProgramData data;
  data.theProgram = ogl::makePorgram({
                                       ogl::Shader(GL_VERTEX_SHADER, strVertexShader),
                                       ogl::Shader(GL_FRAGMENT_SHADER, strFragmentShader)
                                     });
  data.modelToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "modelToCameraMatrix");
  data.objectColorUnif = glGetUniformLocation(data.theProgram, "objectColor");

  GLuint projectionBlock = glGetUniformBlockIndex(data.theProgram, "Projection");
  glUniformBlockBinding(data.theProgram, projectionBlock, mProjectionBlockIndex);

  return data;
}

glm::vec4 FragmentAttenuation::CalcLightPosition()
{
  float fCurrTimeThroughLoop = mLightTimer.GetAlpha();

  glm::vec4 ret(0.0f, mLightHeight, 0.0f, 1.0f);

  ret.x = std::cos(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * mLightRadius;
  ret.z = std::sin(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * mLightRadius;

  return ret;
}

void FragmentAttenuation::framebufferSizeChanged(int w, int h)
{
  glutil::MatrixStack persMatrix;
  persMatrix.Perspective(45.0f, (w / static_cast<float>(h)), g_fzNear, g_fzFar);

  UnProjectionBlock unprojData;
  unprojData.clipToCameraMatrix = glm::inverse(persMatrix.Top());
  unprojData.windowSize = glm::ivec2(w, h);

  glBindBuffer(GL_UNIFORM_BUFFER, mProjectionUniformBuffer);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(persMatrix.Top()));
  glBindBuffer(GL_UNIFORM_BUFFER, mUnprojectionUniformBuffer);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UnProjectionBlock), &unprojData);

  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glViewport(0, 0, w, h);
}

void FragmentAttenuation::renderInternal()
{
  mLightTimer.Update();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutil::MatrixStack modelMatrix;
  modelMatrix.SetMatrix(mViewPole.CalcMatrix());

  const glm::vec4& worldLightPos = CalcLightPosition();
  glm::vec4 lightPosCameraSpace = modelMatrix.Top() * worldLightPos;

  ProgramData* whiteProgram = &mWhiteFragmentDiffuse;
  ProgramData* colorProgram = &mColorFragmentDiffuse;

  glUseProgram(whiteProgram->theProgram);
  glUniform4f(whiteProgram->lightIntensityUnif, 0.8f, 0.8f, 0.8f, 1.0f);
  glUniform4f(whiteProgram->ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
  glUniform3fv(whiteProgram->cameraSpaceLightPosUnif, 1, glm::value_ptr(lightPosCameraSpace));
  glUniform1i(whiteProgram->bUseRSquareUnif, mUseRSquare ? 1 : 0);
  glUniform1f(whiteProgram->lightAttenuationUnif, mLightAttenuation);
  glUseProgram(colorProgram->theProgram);
  glUniform4f(colorProgram->lightIntensityUnif, 0.8f, 0.8f, 0.8f, 1.0f);
  glUniform4f(colorProgram->ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
  glUniform3fv(colorProgram->cameraSpaceLightPosUnif, 1, glm::value_ptr(lightPosCameraSpace));
  glUniform1i(colorProgram->bUseRSquareUnif, mUseRSquare ? 1 : 0);
  glUniform1f(colorProgram->lightAttenuationUnif, mLightAttenuation);
  glUseProgram(0);

  {
    glutil::PushStack push(modelMatrix);

    //Render the ground plane.
    {
      glutil::PushStack push(modelMatrix);

      glUseProgram(whiteProgram->theProgram);
      glUniformMatrix4fv(whiteProgram->modelToCameraMatrixUnif, 1, GL_FALSE,
        glm::value_ptr(modelMatrix.Top()));
      glm::mat3 normMatrix(modelMatrix.Top());
      glUniformMatrix3fv(whiteProgram->normalModelToCameraMatrixUnif, 1, GL_FALSE,
        glm::value_ptr(normMatrix));

      mPlane.Render();
      glUseProgram(0);
    }

    //Render the Cylinder
    {
      glutil::PushStack push(modelMatrix);

      modelMatrix.ApplyMatrix(mObjtPole.CalcMatrix());
      if (mScaleCylinder) {
        modelMatrix.Scale(1.0f, 1.0f, 0.2f);
      }

      if(mDrawColoredCyl)
      {
        glUseProgram(colorProgram->theProgram);
        glUniformMatrix4fv(colorProgram->modelToCameraMatrixUnif, 1, GL_FALSE,
          glm::value_ptr(modelMatrix.Top()));
        glm::mat3 normMatrix(modelMatrix.Top());
        glUniformMatrix3fv(colorProgram->normalModelToCameraMatrixUnif, 1, GL_FALSE,
          glm::value_ptr(normMatrix));

        mCylinder.Render("lit-color");
      }
      else
      {
        glUseProgram(whiteProgram->theProgram);
        glUniformMatrix4fv(whiteProgram->modelToCameraMatrixUnif, 1, GL_FALSE,
          glm::value_ptr(modelMatrix.Top()));
        glm::mat3 normMatrix(modelMatrix.Top());
        glUniformMatrix3fv(whiteProgram->normalModelToCameraMatrixUnif, 1, GL_FALSE,
          glm::value_ptr(normMatrix));

        mCylinder.Render("lit");
      }
      glUseProgram(0);
    }

    //Render the light
    if(mDrawLight)
    {
      glutil::PushStack push(modelMatrix);

      modelMatrix.Translate(glm::vec3(worldLightPos));
      modelMatrix.Scale(0.1f, 0.1f, 0.1f);

      glUseProgram(mUnlit.theProgram);
      glUniformMatrix4fv(mUnlit.modelToCameraMatrixUnif, 1, GL_FALSE,
        glm::value_ptr(modelMatrix.Top()));
      glUniform4f(mUnlit.objectColorUnif, 0.8078f, 0.8706f, 0.9922f, 1.0f);
      mCube.Render("flat");
    }
  }
}

void FragmentAttenuation::MouseMotion(int x, int y)
{
  Framework::ForwardMouseMotion(mViewPole, x, y);
  Framework::ForwardMouseMotion(mObjtPole, x, y);
}

void FragmentAttenuation::MouseButton(int button, int state, int mods)
{
  Framework::ForwardMouseButton(mViewPole, mWindow, button, state, mods);
  Framework::ForwardMouseButton(mObjtPole, mWindow, button, state, mods);
}

void FragmentAttenuation::MouseWheel(int offset)
{
  Framework::ForwardMouseWheel(mViewPole, mWindow, offset);
  Framework::ForwardMouseWheel(mObjtPole, mWindow, offset);
}

void FragmentAttenuation::onKeyboard(int key, Window::Action act, int mods)
{
  if (act == Window::RELEASE) {
    return;
  }

  float offset = (mods & Window::MOD_SHIFT) ? 0.05f : 0.2f;
  float attenFactor = (mods & Window::MOD_SHIFT) ? 1.1f : 1.5f;

  switch (key) {

  case 32: mDrawColoredCyl = !mDrawColoredCyl; break;

  case 'I': mLightHeight += offset; break;
  case 'K': mLightHeight -= offset; break;
  case 'L': mLightRadius += offset; break;
  case 'J': mLightRadius -= offset; break;

  case 'O': mLightAttenuation *= attenFactor; break;
  case 'U': mLightAttenuation /= attenFactor; break;

  case 'Y': mDrawLight = !mDrawLight; break;
  case 'B': mLightTimer.TogglePause(); break;
  case 'T': mScaleCylinder = !mScaleCylinder; break;
  case 'H':
    mUseRSquare = !mUseRSquare;
    rlzLog(Log::INFO, (mUseRSquare ? "Inverse Squared Attenuation" : "Plain Inverse Attenuation"));
    break;
  }

  mLightRadius = glm::max(mLightRadius, 0.2f);
  mLightAttenuation = glm::max(mLightAttenuation, 0.1f);
}
