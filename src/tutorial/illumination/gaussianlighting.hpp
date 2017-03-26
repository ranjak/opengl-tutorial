#ifndef GAUSSIANLIGHTING_HPP
#define GAUSSIANLIGHTING_HPP

#include "tutorial.hpp"
#include "window.hpp"
#include "Mesh.h"
#include "framework/Timer.h"
#include <glutil/MousePoles.h>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <string>

struct ProgramData
{
  GLuint theProgram;

  GLint modelToCameraMatrixUnif;
  GLint normalModelToCameraMatrixUnif;

  GLint cameraSpaceLightPosUnif;
  GLint lightIntensityUnif;
  GLint ambientIntensityUnif;
  GLint lightAttenuationUnif;
  GLint shininessFactorUnif;

  // Only for single-color (white) shader
  GLint baseDiffuseColorUnif;
};

struct UnlitProgramData
{
  GLuint theProgram;

  GLint objectColorUnif;
  GLint modelToCameraMatrixUnif;
};


class GaussianLighting : public Tutorial
{
public:
  GaussianLighting(Window* window);
private:
  void framebufferSizeChanged(int w, int h) override;
  void renderInternal() override;

  ProgramData LoadProgram(const std::string& strVertexShader, const std::string& strFragmentShader);
  UnlitProgramData LoadUnlitProgram(const std::string& strVertexShader, const std::string& strFragmentShader);
  glm::vec4 CalcLightPosition();

  void MouseMotion(int x, int y);
  void MouseButton(int button, int state, int mods);
  void MouseWheel(int offset);
  void onKeyboard(int key, Window::Action act, int mods);

private:
  const GLint mProjectionBlockIndex;
  GLuint mProjectionUniformBuffer;

  ProgramData mWhitePhongDiffuse;
  ProgramData mColorPhongDiffuse;
  ProgramData mWhitePhongOnly;
  ProgramData mColorPhongOnly;
  ProgramData mWhiteBlinnDiffuse;
  ProgramData mColorBlinnDiffuse;
  ProgramData mWhiteBlinnOnly;
  ProgramData mColorBlinnOnly;
  ProgramData mWhiteGaussianDiffuse;
  ProgramData mColorGaussianDiffuse;
  ProgramData mWhiteGaussianOnly;
  ProgramData mColorGaussianOnly;
  UnlitProgramData mUnlit;

  Framework::Mesh mCylinder;
  Framework::Mesh mPlane;
  Framework::Mesh mCube;
  glutil::ViewPole mViewPole;
  glutil::ObjectPole mObjtPole;

  bool mDrawColoredCyl;
  bool mDrawLight;
  bool mScaleCylinder;
  bool mDiffuseEnabled;
  enum { PHONG, BLINN, GAUSS } mShaderType;
  float mLightHeight;
  float mLightRadius;
  Framework::Timer mLightTimer;
  float mLightAttenuation;
  float mPhongShininessFactor;
  float mBlinnShininessFactor;
  float mGaussRoughness;
  glm::vec4 mDiffuseColor;
};

#endif // GAUSSIANLIGHTING_HPP
