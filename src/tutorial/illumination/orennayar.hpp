#ifndef ORENNAYAR_HPP
#define ORENNAYAR_HPP

#include "tutorial.hpp"
#include "Mesh.h"
#include "window.hpp"
#include <glutil/MousePoles.h>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <string>

struct ProgramData
{
  GLuint theProgram;

  GLint dirToLightUnif;
  GLint lightIntensityUnif;
  GLint ambientIntensityUnif;
  GLint facetAnglesUnif;

  GLint modelToCameraMatrixUnif;
  GLint normalModelToCameraMatrixUnif;
};


class OrenNayar : public Tutorial
{
public:
  OrenNayar(Window* window);

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

  ProgramData mWhiteAmbDiffuseColor;
  ProgramData mVertexAmbDiffuseColor;
  ProgramData mWhiteOrenNayar;
  ProgramData mVertexOrenNayar;

  Framework::Mesh mCylinder;
  Framework::Mesh mPlane;

  bool mDrawColoredCyl;
  bool mUseOrenNayar;
  glutil::ViewPole mViewPole;
  glutil::ObjectPole mObjtPole;
  glm::vec4 mLightIntensity;
  glm::vec2 mONfacetAngles;
};

#endif // ORENNAYAR_HPP
