#include "rotatingtriangle.hpp"
#include "shader.hpp"
#include "primitives.hpp"
#include "util.hpp"
#include "transform.hpp"
#include <glm/vec2.hpp>

namespace
{
// Rotation speed
float degPerSecond = 90.0f;
} // namespace

RotatingTriangle::RotatingTriangle(Window* win) :
  Tutorial(win),
  mProgram(0),
  mBuffer(0),
  mVao(0)
{
  glGenBuffers(1, &mBuffer);

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ogl::coloredTriangle), ogl::coloredTriangle, GL_STREAM_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  mProgram = ogl::makePorgram({
    ogl::Shader(GL_VERTEX_SHADER, "shaders/colored-triangle.vert"),
    ogl::Shader(GL_FRAGMENT_SHADER, "shaders/colored-triangle.frag")
                              });
  glGenVertexArrays(1, &mVao);
}

void RotatingTriangle::update(ogl::seconds delta)
{
  for (int i=0; i<3; ++i) {
    ogl::rotateZ(ogl::coloredTriangle[i], glm::vec2(0.f, 0.f), degPerSecond * delta.count());
  }

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  glBufferSubData(GL_ARRAY_BUFFER, 0, 3*sizeof(ogl::coloredTriangle[0]), ogl::coloredTriangle);
  OGL_CHECK_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RotatingTriangle::renderInternal()
{
  glUseProgram(mProgram);
  OGL_CHECK_ERROR();

  glBindVertexArray(mVao);

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
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
}
