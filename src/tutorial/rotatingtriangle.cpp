#include "rotatingtriangle.hpp"
#include "shader.hpp"
#include "primitives.hpp"
#include "util.hpp"

void RotatingTriangle::init()
{
  glGenBuffers(1, &mBuffer);

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ogl::coloredTriangle), ogl::coloredTriangle, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  mProgram = ogl::makePorgram({
    ogl::Shader(GL_VERTEX_SHADER, "shaders/colored-triangle.vert"),
    ogl::Shader(GL_FRAGMENT_SHADER, "shaders/colored-triangle.frag")
                              });
}

void RotatingTriangle::update()
{

}

void RotatingTriangle::render()
{
  glUseProgram(mProgram);
  OGL_CHECK_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  // Geometry
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  // Colors
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(3*sizeof(ogl::coloredTriangle[0])));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  OGL_CHECK_ERROR();

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  OGL_CHECK_ERROR();

  glDrawArrays(GL_TRIANGLES, 0, 3);
  OGL_CHECK_ERROR();

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glUseProgram(0);
}
