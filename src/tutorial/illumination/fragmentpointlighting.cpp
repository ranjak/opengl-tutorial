#include "fragmentpointlighting.hpp"
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

} // namespace

FragmentPointLighting::FragmentPointLighting(Window* window) :
  Tutorial(window),
  mProjectionBlockIndex(2),
  mProjectionUniformBuffer(0),

  mWhiteDiffuseColor(LoadProgram("shaders/illumination/fragment/posvertexlighting_pn.vert", "shaders/illumination/ColorPassthrough.frag")),
  mVertexDiffuseColor(LoadProgram("shaders/illumination/fragment/posvertexlighting_pcn.vert", "shaders/illumination/ColorPassthrough.frag")),
  mWhiteFragmentDiffuse(LoadProgram("shaders/illumination/fragment/posfragmentlighting_pn.vert", "shaders/illumination/fragment/diffuse_lambert.frag")),
  mColorFragmentDiffuse(LoadProgram("shaders/illumination/fragment/posfragmentlighting_pcn.vert", "shaders/illumination/fragment/diffuse_lambert.frag")),
  mWhiteFragmentDiffuse_ON(LoadProgram("shaders/illumination/fragment/posfragmentlighting_pn.vert", "shaders/illumination/fragment/diffuse_oren-nayar-fujii.frag")),
  mColorFragmentDiffuse_ON(LoadProgram("shaders/illumination/fragment/posfragmentlighting_pcn.vert", "shaders/illumination/fragment/diffuse_oren-nayar-fujii.frag")),
  mUnlit(LoadUnlitProgram("shaders/illumination/PosTransform.vert", "shaders/illumination/UniformColor.frag")),

  mCylinder("assets/illumination/UnitCylinder.xml"),
  mPlane("assets/illumination/LargePlane.xml"),
  mCube("assets/illumination/UnitCube.xml"),
  mDrawColoredCyl(true),
  mDrawLight(true),
  mUseFragmentLighting(false),
  mScaleCylinder(false),
  mUseOrenNayar(false),
  mSurfaceRoughness(0.0f),
  mViewPole(g_initialViewData, g_viewScale, glutil::MB_LEFT_BTN),
  mObjtPole(g_initialObjectData, 90.0f/250.0f, glutil::MB_RIGHT_BTN, &mViewPole),

  mLightHeight(1.5f),
  mLightRadius(1.0f),
  mLightTimer(Framework::Timer::TT_LOOP, 5.0f)
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
    FragmentPointLighting* thisTut = static_cast<FragmentPointLighting*>(win->getTutorial());
    thisTut->MouseMotion(static_cast<int>(x), static_cast<int>(y));
  });
  mWindow->setMouseButtonCallback([] (Window* win, int button, int state, int mods)
  {
    FragmentPointLighting* thisTut = static_cast<FragmentPointLighting*>(win->getTutorial());
    thisTut->MouseButton(button, state, mods);
  });
  mWindow->setScrollCallback([] (Window* win, double /*x*/, double y)
  {
    FragmentPointLighting* thisTut = static_cast<FragmentPointLighting*>(win->getTutorial());
    thisTut->MouseWheel(static_cast<int>(y));
  });
  mWindow->addKeyCallback([] (Window* win, int key, int, Window::Action act, short mods)
  {
    FragmentPointLighting* thisTut = static_cast<FragmentPointLighting*>(win->getTutorial());
    thisTut->onKeyboard(key, act, mods);
  });
}

ProgramData FragmentPointLighting::LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
  ProgramData data;
  data.theProgram = ogl::makePorgram({
                                       ogl::Shader(GL_VERTEX_SHADER, strVertexShader),
                                       ogl::Shader(GL_FRAGMENT_SHADER, strFragmentShader)
                                     });
  data.modelToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "modelToCameraMatrix");
  data.modelSpaceLightPosUnif = glGetUniformLocation(data.theProgram, "lightPos");
  data.lightIntensityUnif = glGetUniformLocation(data.theProgram, "lightIntensity");
  data.ambientIntensityUnif = glGetUniformLocation(data.theProgram, "ambientIntensity");

  data.modelSpaceCamPosUnif = glGetUniformLocation(data.theProgram, "cameraPos");
  data.surfaceRoughnessUnif = glGetUniformLocation(data.theProgram, "sigma");

  GLuint projectionBlock = glGetUniformBlockIndex(data.theProgram, "Projection");
  glUniformBlockBinding(data.theProgram, projectionBlock, mProjectionBlockIndex);

  return data;
}

UnlitProgramData FragmentPointLighting::LoadUnlitProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
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

glm::vec4 FragmentPointLighting::CalcLightPosition()
{
  float fCurrTimeThroughLoop = mLightTimer.GetAlpha();

  glm::vec4 ret(0.0f, mLightHeight, 0.0f, 1.0f);

  ret.x = std::cos(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * mLightRadius;
  ret.z = std::sin(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * mLightRadius;

  return ret;
}

void FragmentPointLighting::framebufferSizeChanged(int w, int h)
{
  glutil::MatrixStack persMatrix;
  persMatrix.Perspective(45.0f, (w / static_cast<float>(h)), g_fzNear, g_fzFar);

  glBindBuffer(GL_UNIFORM_BUFFER, mProjectionUniformBuffer);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(persMatrix.Top()));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glViewport(0, 0, w, h);
}

void FragmentPointLighting::renderInternal()
{
  mLightTimer.Update();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutil::MatrixStack modelMatrix;
  modelMatrix.SetMatrix(mViewPole.CalcMatrix());

  const glm::vec4& worldLightPos = CalcLightPosition();

  glm::vec4 lightPosCameraSpace = modelMatrix.Top() * worldLightPos;
  glm::vec4 camPosCameraSpace(0.0f, 0.0f, 0.0f, 1.0f);

  ProgramData* whiteProgram = &mWhiteDiffuseColor;
  ProgramData* colorProgram = &mVertexDiffuseColor;
  if (mUseFragmentLighting && mUseOrenNayar) {
    whiteProgram = &mWhiteFragmentDiffuse_ON;
    colorProgram = &mColorFragmentDiffuse_ON;
  }
  else if (mUseFragmentLighting) {
    whiteProgram = &mWhiteFragmentDiffuse;
    colorProgram = &mColorFragmentDiffuse;
  }

  glUseProgram(whiteProgram->theProgram);
  glUniform4f(whiteProgram->lightIntensityUnif, 0.8f, 0.8f, 0.8f, 1.0f);
  glUniform4f(whiteProgram->ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
  glUniform1f(whiteProgram->surfaceRoughnessUnif, mSurfaceRoughness);
  glUseProgram(colorProgram->theProgram);
  glUniform4f(colorProgram->lightIntensityUnif, 0.8f, 0.8f, 0.8f, 1.0f);
  glUniform4f(colorProgram->ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
  glUniform1f(colorProgram->surfaceRoughnessUnif, mSurfaceRoughness);
  glUseProgram(0);

  {
    glutil::PushStack push(modelMatrix);

    //Render the ground plane.
    {
      glutil::PushStack push(modelMatrix);

      glUseProgram(whiteProgram->theProgram);
      glUniformMatrix4fv(whiteProgram->modelToCameraMatrixUnif, 1, GL_FALSE,
        glm::value_ptr(modelMatrix.Top()));

      glm::mat4 camToModel = glm::inverse(modelMatrix.Top());
      glm::vec4 modelLightPos = camToModel * lightPosCameraSpace;
      glm::vec4 modelCamPos = camToModel * camPosCameraSpace;
      glUniform3fv(whiteProgram->modelSpaceLightPosUnif, 1, glm::value_ptr(modelLightPos));
      glUniform3fv(whiteProgram->modelSpaceCamPosUnif, 1, glm::value_ptr(modelCamPos));

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

      glm::mat4 camToModel = glm::inverse(modelMatrix.Top());
      glm::vec4 modelLightPos = camToModel * lightPosCameraSpace;
      glm::vec4 modelCamPos = camToModel * camPosCameraSpace;

      if(mDrawColoredCyl)
      {
        glUseProgram(colorProgram->theProgram);
        glUniformMatrix4fv(colorProgram->modelToCameraMatrixUnif, 1, GL_FALSE,
          glm::value_ptr(modelMatrix.Top()));
        glUniform3fv(colorProgram->modelSpaceLightPosUnif, 1, glm::value_ptr(modelLightPos));
        glUniform3fv(colorProgram->modelSpaceCamPosUnif, 1, glm::value_ptr(modelCamPos));

        mCylinder.Render("lit-color");
      }
      else
      {
        glUseProgram(whiteProgram->theProgram);
        glUniformMatrix4fv(whiteProgram->modelToCameraMatrixUnif, 1, GL_FALSE,
          glm::value_ptr(modelMatrix.Top()));
        glUniform3fv(whiteProgram->modelSpaceLightPosUnif, 1, glm::value_ptr(modelLightPos));
        glUniform3fv(whiteProgram->modelSpaceCamPosUnif, 1, glm::value_ptr(modelCamPos));

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

void FragmentPointLighting::MouseMotion(int x, int y)
{
  Framework::ForwardMouseMotion(mViewPole, x, y);
  Framework::ForwardMouseMotion(mObjtPole, x, y);
}

void FragmentPointLighting::MouseButton(int button, int state, int mods)
{
  Framework::ForwardMouseButton(mViewPole, mWindow, button, state, mods);
  Framework::ForwardMouseButton(mObjtPole, mWindow, button, state, mods);
}

void FragmentPointLighting::MouseWheel(int offset)
{
  Framework::ForwardMouseWheel(mViewPole, mWindow, offset);
  Framework::ForwardMouseWheel(mObjtPole, mWindow, offset);
}

void FragmentPointLighting::onKeyboard(int key, Window::Action act, int mods)
{
  if (act == Window::RELEASE) {
    return;
  }

  float offset = (mods & Window::MOD_SHIFT) ? 0.05f : 0.2f;

  switch (key) {

  case 32: mDrawColoredCyl = !mDrawColoredCyl; break;

  case 'I': mLightHeight += offset; break;
  case 'K': mLightHeight -= offset; break;
  case 'L': mLightRadius += offset; break;
  case 'J': mLightRadius -= offset; break;

  case 'Y': mDrawLight = !mDrawLight; break;
  case 'B': mLightTimer.TogglePause(); break;
  case 'H': mUseFragmentLighting = !mUseFragmentLighting; break;
  case 'T': mScaleCylinder = !mScaleCylinder; break;
  case 'O': mUseOrenNayar = !mUseOrenNayar; break;

  case 264: mSurfaceRoughness -= 0.1f; break; // DOWN
  case 265: mSurfaceRoughness += 0.1f; break; // UP
  }

  mSurfaceRoughness = glm::clamp(mSurfaceRoughness, 0.0f, 1.0f);
  if(mLightRadius < 0.2f)
    mLightRadius = 0.2f;
}
