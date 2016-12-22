#include "worldscene.hpp"
#include "shader.hpp"
#include <glutil/MatrixStack.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace
{

ProgramData LoadProgram(const std::string& strVertexShader, const std::string& strFragmentShader)
{
  std::initializer_list<ogl::Shader> shaderList {
    ogl::Shader(GL_VERTEX_SHADER, strVertexShader),
    ogl::Shader(GL_FRAGMENT_SHADER, strFragmentShader)
  };

  ProgramData data;
  data.theProgram = ogl::makePorgram(shaderList);
  data.modelToWorldMatrixUnif = glGetUniformLocation(data.theProgram, "modelToWorldMatrix");
  data.worldToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "worldToCameraMatrix");
  data.cameraToClipMatrixUnif = glGetUniformLocation(data.theProgram, "cameraToClipMatrix");
  data.baseColorUnif = glGetUniformLocation(data.theProgram, "baseColor");

  return data;
}
/*
glm::mat4 CalcLookAtMatrix(const glm::vec3& cameraPt, const glm::vec3& lookPt, const glm::vec3& upPt)
{
  glm::vec3 lookDir = glm::normalize(lookPt - cameraPt);
  glm::vec3 upDir = glm::normalize(upPt);

  glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
  glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

  glm::mat4 rotMat(1.0f);
  rotMat[0] = glm::vec4(rightDir, 0.0f);
  rotMat[1] = glm::vec4(perpUpDir, 0.0f);
  rotMat[2] = glm::vec4(-lookDir, 0.0f);

  rotMat = glm::transpose(rotMat);

  glm::mat4 transMat(1.0f);
  transMat[3] = glm::vec4(-cameraPt, 1.0f);

  return rotMat * transMat;
}
*/
} // namespace

WorldScene::WorldScene(Window* window) :
  Tutorial(window),
  UniformColor(LoadProgram("shaders/world/PosOnlyWorldTransform.vert", "shaders/world/ColorUniform.frag")),
  ObjectColor(LoadProgram("shaders/world/PosColorWorldTransform.vert", "shaders/world/ColorPassthrough.frag")),
  UniformColorTint(LoadProgram("shaders/world/PosColorWorldTransform.vert", "shaders/world/ColorMultUniform.frag")),
  g_pConeMesh("assets/UnitConeTint.xml"),
  g_pCylinderMesh("assets/UnitCylinderTint.xml"),
  g_pCubeTintMesh("assets/UnitCubeTint.xml"),
  g_pCubeColorMesh("assets/UnitCubeColor.xml"),
  g_pPlaneMesh("assets/UnitPlane.xml")
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);
  glEnable(GL_DEPTH_CLAMP);
}

void WorldScene::DrawTree(glutil::MatrixStack& modelMatrix, float fTrunkHeight, float fConeHeight)
{
  //Draw trunk.
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Scale(glm::vec3(1.0f, fTrunkHeight, 1.0f));
    modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

    glUseProgram(UniformColorTint.theProgram);
    glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniform4f(UniformColorTint.baseColorUnif, 0.694f, 0.4f, 0.106f, 1.0f);
    g_pCylinderMesh.Render();
    glUseProgram(0);
  }

  //Draw the treetop
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Translate(glm::vec3(0.0f, fTrunkHeight, 0.0f));
    modelMatrix.Scale(glm::vec3(3.0f, fConeHeight, 3.0f));

    glUseProgram(UniformColorTint.theProgram);
    glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniform4f(UniformColorTint.baseColorUnif, 0.0f, 1.0f, 0.0f, 1.0f);
    g_pConeMesh.Render();
    glUseProgram(0);
  }
}

void WorldScene::renderInternal()
{

}

void WorldScene::framebufferSizeChanged(int /*w*/, int /*h*/)
{

}
