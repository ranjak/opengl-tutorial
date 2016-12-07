#include "movingtriangle.hpp"
#include "transform.hpp"
#include "primitives.hpp"
#include "shader.hpp"
#include "log.hpp"
#include <glm/vec4.hpp>
#include <cmath>

namespace
{
// How much time it takes for the triangle to go full circle
ogl::seconds revolutionTime(5.0f);
}

MovingTriangle::MovingTriangle() :
  mProgram(0),
  mBuffer(0),
  mOffsetUniform(-1),
  mTime(0.0f)
{

}

void MovingTriangle::init()
{
  glGenBuffers(1, &mBuffer);

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ogl::coloredTriangle), ogl::coloredTriangle, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  mProgram = ogl::makePorgram({
    ogl::Shader(GL_VERTEX_SHADER, "shaders/positionoffset.vert"),
    ogl::Shader(GL_FRAGMENT_SHADER, "shaders/colored-triangle.frag")
                              });

  mOffsetUniform = glGetUniformLocation(mProgram, "offset");
  if (mOffsetUniform < 0) {
    Log::error("MovingTriangle: offset uniform has no location");
  }
}

void MovingTriangle::update(ogl::seconds delta)
{
  mTime = static_cast<ogl::seconds>(std::fmod(mTime.count() + delta.count(), revolutionTime.count()));

  float angle = (2.0f*ogl::PIf*mTime.count()) / revolutionTime.count();

  float offsetX = 0.5f * std::cos(angle);
  float offsetY = 0.5f * std::sin(angle);

  glUseProgram(mProgram);
  glUniform2f(mOffsetUniform, offsetX, offsetY);
  glUseProgram(0);
}

void MovingTriangle::render()
{
  glUseProgram(mProgram);

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  // Geometry
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  // Colors
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(3*sizeof(ogl::coloredTriangle[0])));
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
}
