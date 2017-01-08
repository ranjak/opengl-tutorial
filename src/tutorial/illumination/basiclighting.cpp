#include "basiclighting.hpp"
#include "shader.hpp"
#include <glutil/MatrixStack.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

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

} // namespace

ProgramData BasicLighting::LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
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

  GLuint projectionBlock = glGetUniformBlockIndex(data.theProgram, "Projection");
  glUniformBlockBinding(data.theProgram, projectionBlock, mProjectionBlockIndex);

  return data;
}

BasicLighting::BasicLighting(Window* window) :
  Tutorial(window),
  mProjectionBlockIndex(2),
  mProjectionUniformBuffer(0),
  mWhiteDiffuseColor(LoadProgram("shaders/illumination/DirVertexLighting_PN.vert", "shaders/illumination/ColorPassthrough.frag")),
  mVertexDiffuseColor(LoadProgram("shaders/illumination/DirVertexLighting_PCN.vert", "shaders/illumination/ColorPassthrough.frag")),
  mCylinder("assets/illumination/UnitCylinder.xml"),
  mPlane("assets/illumination/LargePlane.xml"),
  mDrawColoredCyl(true),
  mViewPole(g_initialViewData, g_viewScale, glutil::MB_LEFT_BTN),
  mObjtPole(g_initialObjectData, 90.0f/250.0f, glutil::MB_RIGHT_BTN, &mViewPole)
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
}

void BasicLighting::MouseMotion(int x, int y)
{
  Framework::ForwardMouseMotion(g_viewPole, x, y);
  Framework::ForwardMouseMotion(g_objtPole, x, y);
  glutPostRedisplay();
}

void BasicLighting::MouseButton(int button, int state, int x, int y)
{
  Framework::ForwardMouseButton(g_viewPole, button, state, x, y);
  Framework::ForwardMouseButton(g_objtPole, button, state, x, y);
  glutPostRedisplay();
}

void BasicLighting::MouseWheel(int wheel, int direction, int x, int y)
{
  Framework::ForwardMouseWheel(g_viewPole, wheel, direction, x, y);
  Framework::ForwardMouseWheel(g_objtPole, wheel, direction, x, y);
  glutPostRedisplay();
}
