#ifndef FRAGMENTATTENUATION_HPP
#define FRAGMENTATTENUATION_HPP

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

  GLint modelToCameraMatrixUnif;

  GLint lightIntensityUnif;
  GLint ambientIntensityUnif;

  GLint normalModelToCameraMatrixUnif;
  GLint cameraSpaceLightPosUnif;
  GLint windowSizeUnif;
  GLint lightAttenuationUnif;
  GLint bUseRSquareUnif;
};

struct UnlitProgramData
{
  GLuint theProgram;

  GLint objectColorUnif;
  GLint modelToCameraMatrixUnif;
};

class FragmentAttenuation : public Tutorial
{
public:
  FragmentAttenuation(Window* window);
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
  // Projection and Un-projection matrices uniform blocks
  const GLint mProjectionBlockIndex;
  const GLint mUnprojectionBlockIndex;
  GLuint mProjectionUniformBuffer;
  GLuint mUnprojectionUniformBuffer;

  ProgramData mWhiteFragmentDiffuse;
  ProgramData mColorFragmentDiffuse;
  UnlitProgramData mUnlit;

  Framework::Mesh mCylinder;
  Framework::Mesh mPlane;
  Framework::Mesh mCube;

  bool mDrawColoredCyl;
  bool mDrawLight;
  bool mScaleCylinder;
  bool mUseRSquare;
  glutil::ViewPole mViewPole;
  glutil::ObjectPole mObjtPole;

  float mLightHeight;
  float mLightRadius;
  Framework::Timer mLightTimer;
  float mLightAttenuation;
};

#endif // FRAGMENTATTENUATION_HPP
