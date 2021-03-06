#include "gaussianlighting.hpp"
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

GaussianLighting::GaussianLighting(Window* window) :
  Tutorial(window),
  mProjectionBlockIndex(2),
  mProjectionUniformBuffer(0),

  mWhitePhongDiffuse(LoadProgram("shaders/illumination/specular/PN.vert", "shaders/illumination/specular/PhongLighting.frag")),
  mColorPhongDiffuse(LoadProgram("shaders/illumination/specular/PCN.vert", "shaders/illumination/specular/PhongLighting.frag")),
  mWhitePhongOnly(LoadProgram("shaders/illumination/specular/PN.vert", "shaders/illumination/specular/PhongOnly.frag")),
  mColorPhongOnly(LoadProgram("shaders/illumination/specular/PCN.vert", "shaders/illumination/specular/PhongOnly.frag")),
  mWhiteBlinnDiffuse(LoadProgram("shaders/illumination/specular/PN.vert", "shaders/illumination/specular/BlinnLighting.frag")),
  mColorBlinnDiffuse(LoadProgram("shaders/illumination/specular/PCN.vert", "shaders/illumination/specular/BlinnLighting.frag")),
  mWhiteBlinnOnly(LoadProgram("shaders/illumination/specular/PN.vert", "shaders/illumination/specular/BlinnOnly.frag")),
  mColorBlinnOnly(LoadProgram("shaders/illumination/specular/PCN.vert", "shaders/illumination/specular/BlinnOnly.frag")),
  mWhiteGaussianDiffuse(LoadProgram("shaders/illumination/specular/PN.vert", "shaders/illumination/specular/GaussianLighting.frag")),
  mColorGaussianDiffuse(LoadProgram("shaders/illumination/specular/PCN.vert", "shaders/illumination/specular/GaussianLighting.frag")),
  mWhiteGaussianOnly(LoadProgram("shaders/illumination/specular/PN.vert", "shaders/illumination/specular/GaussianOnly.frag")),
  mColorGaussianOnly(LoadProgram("shaders/illumination/specular/PCN.vert", "shaders/illumination/specular/GaussianOnly.frag")),
  mUnlit(LoadUnlitProgram("shaders/illumination/PosTransform.vert", "shaders/illumination/UniformColor.frag")),

  mCylinder("assets/illumination/UnitCylinder.xml"),
  mPlane("assets/illumination/LargePlane.xml"),
  mCube("assets/illumination/UnitCube.xml"),
  mViewPole(g_initialViewData, g_viewScale, glutil::MB_LEFT_BTN),
  mObjtPole(g_initialObjectData, 90.0f/250.0f, glutil::MB_RIGHT_BTN, &mViewPole),

  mDrawColoredCyl(true),
  mDrawLight(true),
  mScaleCylinder(false),
  mDiffuseEnabled(true),
  mShaderType(PHONG),
  mLightHeight(1.5f),
  mLightRadius(1.0f),
  mLightTimer(Framework::Timer::TT_LOOP, 5.0f),
  mLightAttenuation(1.2f),
  mPhongShininessFactor(4.0f),
  mBlinnShininessFactor(6.0f),
  mGaussRoughness(0.5f),
  mDiffuseColor(1.0f)
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
    GaussianLighting* thisTut = static_cast<GaussianLighting*>(win->getTutorial());
    thisTut->MouseMotion(static_cast<int>(x), static_cast<int>(y));
  });
  mWindow->setMouseButtonCallback([] (Window* win, int button, int state, int mods)
  {
    GaussianLighting* thisTut = static_cast<GaussianLighting*>(win->getTutorial());
    thisTut->MouseButton(button, state, mods);
  });
  mWindow->setScrollCallback([] (Window* win, double /*x*/, double y)
  {
    GaussianLighting* thisTut = static_cast<GaussianLighting*>(win->getTutorial());
    thisTut->MouseWheel(static_cast<int>(y));
  });
  mWindow->addKeyCallback([] (Window* win, int key, int, Window::Action act, short mods)
  {
    GaussianLighting* thisTut = static_cast<GaussianLighting*>(win->getTutorial());
    thisTut->onKeyboard(key, act, mods);
  });
}

ProgramData GaussianLighting::LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
  ProgramData data;
  data.theProgram = ogl::makePorgram({
                                       ogl::Shader(GL_VERTEX_SHADER, strVertexShader),
                                       ogl::Shader(GL_FRAGMENT_SHADER, strFragmentShader)
                                     });
  data.modelToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "modelToCameraMatrix");
  data.normalModelToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "normalModelToCameraMatrix");

  data.cameraSpaceLightPosUnif = glGetUniformLocation(data.theProgram, "cameraSpaceLightPos");
  data.lightIntensityUnif = glGetUniformLocation(data.theProgram, "lightIntensity");
  data.ambientIntensityUnif = glGetUniformLocation(data.theProgram, "ambientIntensity");
  data.lightAttenuationUnif = glGetUniformLocation(data.theProgram, "lightAttenuation");
  data.shininessFactorUnif = glGetUniformLocation(data.theProgram, "shininessFactor");

  data.baseDiffuseColorUnif = glGetUniformLocation(data.theProgram, "baseDiffuseColor");

  GLuint projectionBlock = glGetUniformBlockIndex(data.theProgram, "Projection");
  glUniformBlockBinding(data.theProgram, projectionBlock, mProjectionBlockIndex);

  return data;
}

UnlitProgramData GaussianLighting::LoadUnlitProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
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

glm::vec4 GaussianLighting::CalcLightPosition()
{
  float fCurrTimeThroughLoop = mLightTimer.GetAlpha();

  glm::vec4 ret(0.0f, mLightHeight, 0.0f, 1.0f);

  ret.x = std::cos(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * mLightRadius;
  ret.z = std::sin(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * mLightRadius;

  return ret;
}

void GaussianLighting::framebufferSizeChanged(int w, int h)
{
  glutil::MatrixStack persMatrix;
  persMatrix.Perspective(45.0f, (w / static_cast<float>(h)), g_fzNear, g_fzFar);

  glBindBuffer(GL_UNIFORM_BUFFER, mProjectionUniformBuffer);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(persMatrix.Top()));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glViewport(0, 0, w, h);
}

void GaussianLighting::MouseMotion(int x, int y)
{
  Framework::ForwardMouseMotion(mViewPole, x, y);
  Framework::ForwardMouseMotion(mObjtPole, x, y);
}

void GaussianLighting::MouseButton(int button, int state, int mods)
{
  Framework::ForwardMouseButton(mViewPole, mWindow, button, state, mods);
  Framework::ForwardMouseButton(mObjtPole, mWindow, button, state, mods);
}

void GaussianLighting::MouseWheel(int offset)
{
  Framework::ForwardMouseWheel(mViewPole, mWindow, offset);
  Framework::ForwardMouseWheel(mObjtPole, mWindow, offset);
}

void GaussianLighting::onKeyboard(int key, Window::Action act, int mods)
{
  if (act == Window::RELEASE) {
    return;
  }

  float offset = (mods & Window::MOD_SHIFT) ? 0.05f : 0.2f;

  float shineOffset;
  if (mShaderType == GAUSS) {
    shineOffset = (mods & Window::MOD_SHIFT) ? 0.01f : 0.1f;
  }
  else {
    shineOffset = (mods & Window::MOD_SHIFT) ? 0.1f : 0.5f;
  }

  float& currentFactor = [this]() -> float& {
    using namespace std::string_literals;
    switch (mShaderType) {
    case PHONG: return mPhongShininessFactor;
    case BLINN: return mBlinnShininessFactor;
    case GAUSS: return mGaussRoughness;
    default: throw std::runtime_error("Unknown shader type: "s + std::to_string(mShaderType));
    }
  }();

  switch (key) {

  case 32: mDrawColoredCyl = !mDrawColoredCyl; break;

  case 'I': mLightHeight += offset; break;
  case 'K': mLightHeight -= offset; break;
  case 'L': mLightRadius += offset; break;
  case 'J': mLightRadius -= offset; break;
  case 'U': currentFactor -= shineOffset; break;
  case 'O': currentFactor += shineOffset; break;

  case 'Y': mDrawLight = !mDrawLight; break;
  case 'B': mLightTimer.TogglePause(); break;
  case 'T': mScaleCylinder = !mScaleCylinder; break;
  case 'H':
    if (mods & Window::MOD_SHIFT) {
      mDiffuseEnabled = !mDiffuseEnabled;
    }
    else {
      mShaderType = static_cast<decltype(mShaderType)>((mShaderType + 1) % (GAUSS+1));
      rlzLog(Log::INFO, "Shader Type: " << mShaderType);
    }
    break;
  case 'G':
    mDiffuseColor = (mDiffuseColor.x == 1.0f) ? glm::vec4(0.2f, 0.2f, 0.2f, 1.0f) : glm::vec4(1.0f);
    break;
  }

  mLightRadius = glm::max(0.2f, mLightRadius);
  currentFactor = glm::max(0.0001f, currentFactor);
  rlzLog(Log::INFO, "Shininess Factor: " << currentFactor);
}

void GaussianLighting::renderInternal()
{
  mLightTimer.Update();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutil::MatrixStack modelMatrix;
  modelMatrix.SetMatrix(mViewPole.CalcMatrix());

  const glm::vec4& worldLightPos = CalcLightPosition();

  glm::vec4 lightPosCameraSpace = modelMatrix.Top() * worldLightPos;

  ProgramData* whiteProgram = nullptr;
  ProgramData* colorProgram = nullptr;
  float* currentFactor = nullptr;

  switch (mShaderType) {

  case PHONG:
    whiteProgram = mDiffuseEnabled ? &mWhitePhongDiffuse : &mWhitePhongOnly;
    colorProgram = mDiffuseEnabled ? &mColorPhongDiffuse : &mColorPhongOnly;
    currentFactor = &mPhongShininessFactor;
    break;
  case BLINN:
    whiteProgram = mDiffuseEnabled ? &mWhiteBlinnDiffuse : &mWhiteBlinnOnly;
    colorProgram = mDiffuseEnabled ? &mColorBlinnDiffuse : &mColorBlinnOnly;
    currentFactor = &mBlinnShininessFactor;
    break;
  case GAUSS:
    whiteProgram = mDiffuseEnabled ? &mWhiteGaussianDiffuse : &mWhiteGaussianOnly;
    colorProgram = mDiffuseEnabled ? &mColorGaussianDiffuse : &mColorGaussianOnly;
    currentFactor = &mGaussRoughness;
    break;
  }
  glUseProgram(whiteProgram->theProgram);
  glUniform4f(whiteProgram->lightIntensityUnif, 0.8f, 0.8f, 0.8f, 1.0f);
  glUniform4f(whiteProgram->ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
  glUniform3fv(whiteProgram->cameraSpaceLightPosUnif, 1, glm::value_ptr(lightPosCameraSpace));
  glUniform1f(whiteProgram->lightAttenuationUnif, mLightAttenuation);
  glUniform1f(whiteProgram->shininessFactorUnif, *currentFactor);
  glUniform4fv(whiteProgram->baseDiffuseColorUnif, 1, glm::value_ptr(mDiffuseColor));

  glUseProgram(colorProgram->theProgram);
  glUniform4f(colorProgram->lightIntensityUnif, 0.8f, 0.8f, 0.8f, 1.0f);
  glUniform4f(colorProgram->ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
  glUniform3fv(colorProgram->cameraSpaceLightPosUnif, 1, glm::value_ptr(lightPosCameraSpace));
  glUniform1f(colorProgram->lightAttenuationUnif, mLightAttenuation);
  glUniform1f(colorProgram->shininessFactorUnif, *currentFactor);
  glUseProgram(0);

  {
    glutil::PushStack push(modelMatrix);

    //Render the ground plane.
    {
      glutil::PushStack push(modelMatrix);

      glUseProgram(whiteProgram->theProgram);
      glUniformMatrix4fv(whiteProgram->modelToCameraMatrixUnif, 1, GL_FALSE,
        glm::value_ptr(modelMatrix.Top()));

      glm::mat3 normMatrixInverse(modelMatrix.TopInverse());
      normMatrixInverse = glm::transpose(normMatrixInverse);
      glUniformMatrix3fv(whiteProgram->normalModelToCameraMatrixUnif, 1, GL_FALSE,
                         glm::value_ptr(normMatrixInverse));
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

      ProgramData& program = mDrawColoredCyl ? *colorProgram : *whiteProgram;

      glUseProgram(program.theProgram);
      glUniformMatrix4fv(program.modelToCameraMatrixUnif, 1, GL_FALSE,
                         glm::value_ptr(modelMatrix.Top()));

      glm::mat3 normMatrixInverse(modelMatrix.TopInverse());
      normMatrixInverse = glm::transpose(normMatrixInverse);
      glUniformMatrix3fv(program.normalModelToCameraMatrixUnif, 1, GL_FALSE,
                         glm::value_ptr(normMatrixInverse));

      if (mDrawColoredCyl) {
        mCylinder.Render("lit-color");
      }
      else {
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
