#include "movingtriangleshader.hpp"
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

MovingTriangleShader::MovingTriangleShader(Window* win) :
  Tutorial(win),
  mProgram(0),
  mBuffer(0),
  mVao(0),
  mTimeUniform(-1),
  mTime(0.0f)
{
  glGenBuffers(1, &mBuffer);

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ogl::coloredTriangle), ogl::coloredTriangle, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  mProgram = ogl::makePorgram({
    ogl::Shader(GL_VERTEX_SHADER, "shaders/positionoffset-calc.vert"),
    ogl::Shader(GL_FRAGMENT_SHADER, "shaders/colored-triangle-calc.frag")
                              });

  mTimeUniform = glGetUniformLocation(mProgram, "currentTime");
  GLint piUniform = glGetUniformLocation(mProgram, "pi");
  GLint loopUniform = glGetUniformLocation(mProgram, "loopDuration");
  GLint fragLoopUniform = glGetUniformLocation(mProgram, "fadeLoopDuration");

  if (mTimeUniform < 0) {
    Log::error("MovingTriangleShader: time uniform has no location");
  }
  if (piUniform < 0) {
    Log::error("MovingTriangleShader: pi uniform has no location");
  }
  if (loopUniform < 0) {
    Log::error("MovingTriangleShader: loopDuration uniform has no location");
  }

  // Initialize constant uniforms
  glUseProgram(mProgram);
  glUniform1f(piUniform, ogl::PIf);
  glUniform1f(loopUniform, revolutionTime.count());
  glUniform1f(fragLoopUniform, revolutionTime.count() / 2.0f);
  glUseProgram(0);

  glGenVertexArrays(1, &mVao);
}

void MovingTriangleShader::update(ogl::seconds delta)
{
  mTime = static_cast<ogl::seconds>(std::fmod(mTime.count() + delta.count(), revolutionTime.count()));
}

void MovingTriangleShader::renderInternal()
{
  glUseProgram(mProgram);

  // Update time
  glUniform1f(mTimeUniform, mTime.count());

  glBindVertexArray(mVao);

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  // Geometry
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  // Colors
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(3*sizeof(ogl::coloredTriangle[0])));
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Draw a second triangle opposite to the first
  glUniform1f(mTimeUniform, mTime.count() + revolutionTime.count() / 2.0f);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
}
