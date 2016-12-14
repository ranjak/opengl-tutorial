#include "orthographicprism.hpp"
#include "primitives.hpp"
#include "shader.hpp"


OrthographicPrism::OrthographicPrism(Window* win) :
  Tutorial(win),
  mProgram(0),
  mBuffer(0),
  mVao(0)
{
  glGenBuffers(1, &mBuffer);

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ogl::coloredPrism), ogl::coloredPrism, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  mProgram = ogl::makePorgram({
                                ogl::Shader(GL_VERTEX_SHADER, "shaders/positionoffset.vert"),
                                ogl::Shader(GL_FRAGMENT_SHADER, "shaders/colored-triangle.frag")
                              });

  GLint offsetUniform = glGetUniformLocation(mProgram, "offset");
  glUseProgram(mProgram);
  glUniform2f(offsetUniform, 0.5f, 0.25f);
  glUseProgram(0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glGenVertexArrays(1, &mVao);
}

void OrthographicPrism::renderInternal()
{
  glUseProgram(mProgram);

  glBindVertexArray(mVao);

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  // Geometry
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  // Colors
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(sizeof(ogl::coloredPrism) / 2));
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
}
