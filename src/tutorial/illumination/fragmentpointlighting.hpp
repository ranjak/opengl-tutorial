#ifndef FRAGMENTPOINTLIGHTING_HPP
#define FRAGMENTPOINTLIGHTING_HPP

#include "tutorial.hpp"
#include "window.hpp"
#include "Mesh.h"
#include "framework/Timer.h"
#include <glutil/MousePoles.h>
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <string>

struct ProgramData
{
  GLuint theProgram;

  GLint modelSpaceLightPosUnif;
  GLint lightIntensityUnif;
  GLint ambientIntensityUnif;
  GLint modelToCameraMatrixUnif;

  // Oren-Nayar-specific
  GLint modelSpaceCamPosUnif;
  GLint surfaceRoughnessUnif;
};

struct UnlitProgramData
{
  GLuint theProgram;

  GLint objectColorUnif;
  GLint modelToCameraMatrixUnif;
};

class FragmentPointLighting : public Tutorial
{
public:
  FragmentPointLighting(Window* window);
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
  // Projection matrix uniform block
  const GLint mProjectionBlockIndex;
  GLuint mProjectionUniformBuffer;

  ProgramData mWhiteDiffuseColor;
  ProgramData mVertexDiffuseColor;
  ProgramData mWhiteFragmentDiffuse;
  ProgramData mColorFragmentDiffuse;
  ProgramData mWhiteFragmentDiffuse_ON;
  ProgramData mColorFragmentDiffuse_ON;
  UnlitProgramData mUnlit;

  Framework::Mesh mCylinder;
  Framework::Mesh mPlane;
  Framework::Mesh mCube;

  bool mDrawColoredCyl;
  bool mDrawLight;
  bool mUseFragmentLighting;
  bool mScaleCylinder;
  bool mUseOrenNayar;
  float mSurfaceRoughness;
  glutil::ViewPole mViewPole;
  glutil::ObjectPole mObjtPole;

  float mLightHeight;
  float mLightRadius;
  Framework::Timer mLightTimer;
};

#endif // FRAGMENTPOINTLIGHTING_HPP
