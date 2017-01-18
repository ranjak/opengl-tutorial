#ifndef PHONGLIGHTING_HPP
#define PHONGLIGHTING_HPP

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


class PhongLighting : public Tutorial
{
public:
  PhongLighting(Window* window);

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
  enum LightingMode
  {
    DIFFUSE_ONLY,
    DIFFUSE_AND_PHONG,
    PHONG_ONLY,
    NUM_MODES
  };

  const GLint mProjectionBlockIndex;
  GLuint mProjectionUniformBuffer;

  ProgramData mWhitePhongDiffuse;
  ProgramData mColorPhongDiffuse;
  ProgramData mWhitePhongOnly;
  ProgramData mColorPhongOnly;
  ProgramData mWhiteNoPhong;
  ProgramData mColorNoPhong;
  UnlitProgramData mUnlit;

  Framework::Mesh mCylinder;
  Framework::Mesh mPlane;
  Framework::Mesh mCube;
  glutil::ViewPole mViewPole;
  glutil::ObjectPole mObjtPole;

  bool mDrawColoredCyl;
  bool mDrawLight;
  bool mScaleCylinder;
  int mLightMode;
  float mLightHeight;
  float mLightRadius;
  Framework::Timer mLightTimer;
  float mLightAttenuation;
  float mShininessFactor;
  glm::vec4 mDiffuseColor;
};

#endif // PHONGLIGHTING_HPP
