#include "worldscene.hpp"
#include "shader.hpp"

namespace
{

ProgramData LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
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

} // namespace

WorldScene::WorldScene(Window *window) :
  Tutorial(window),
  UniformColor(LoadProgram("shaders/world/PosOnlyWorldTransform.vert", "shaders/world/ColorUniform.frag")),
  ObjectColor(LoadProgram("shaders/world/PosColorWorldTransform.vert", "shaders/world/ColorPassthrough.frag")),
  UniformColorTint(LoadProgram("shaders/world/PosColorWorldTransform.vert", "shaders/world/ColorMultUniform.frag"))
{

}

void WorldScene::renderInternal()
{

}

void WorldScene::framebufferSizeChanged(int /*w*/, int /*h*/)
{

}
