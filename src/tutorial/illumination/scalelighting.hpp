#ifndef SCALELIGHTING_HPP
#define SCALELIGHTING_HPP

#include "tutorial.hpp"
#include "Mesh.h"
#include "window.hpp"
#include <glutil/MousePoles.h>
#include <glad/glad.h>
#include <string>

struct ProgramData
{
  GLuint theProgram;

  GLuint dirToLightUnif;
  GLuint lightIntensityUnif;

  GLuint modelToCameraMatrixUnif;
  GLuint normalModelToCameraMatrixUnif;
};

class ScaleLighting : public Tutorial
{
public:
  ScaleLighting(Window* win);

private:
  void framebufferSizeChanged(int w, int h) override;
  void renderInternal() override;

  ProgramData LoadProgram(const std::string& strVertexShader, const std::string& strFragmentShader);

  void MouseMotion(int x, int y);
  void MouseButton(int button, int state, int mods);
  void MouseWheel(int offset);
  void onKeyboard(int key, Window::Action act);

private:
  // Projection matrix uniform block
  const GLint mProjectionBlockIndex;
  GLuint mProjectionUniformBuffer;

  ProgramData mWhiteDiffuseColor;
  ProgramData mVertexDiffuseColor;

  Framework::Mesh mCylinder;
  Framework::Mesh mPlane;

  bool mDrawScaledCyl;
  bool mUseInverseTransp;
  glutil::ViewPole mViewPole;
  glutil::ObjectPole mObjtPole;
};

#endif // SCALELIGHTING_HPP
