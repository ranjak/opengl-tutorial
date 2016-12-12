#include "matrixperspective.hpp"
#include "primitives.hpp"
#include "shader.hpp"
#include "util.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{

glm::mat4 makeProjectionMat(float zNear, float zFar, float frustumScale)
{
  return glm::mat4 {
    frustumScale, 0.0, 0.0, 0.0,
    0.0, frustumScale, 0.0, 0.0,
    0.0, 0.0, (-zNear - zFar)/(zNear - zFar), -1.0,
    0.0, 0.0,  2*zNear*zFar/(zNear - zFar), 0.0
  };
}

} // namespace

void MatrixPerspective::init()
{
  glGenBuffers(1, &mBuffer);

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ogl::perspectivePrism), ogl::perspectivePrism, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  mProgram = ogl::makePorgram({
                                ogl::Shader(GL_VERTEX_SHADER, "shaders/matrixperspective.vert"),
                                ogl::Shader(GL_FRAGMENT_SHADER, "shaders/colored-triangle.frag")
                              });

  GLint offsetUniform = glGetUniformLocation(mProgram, "offset");
  GLint projMatUniform = glGetUniformLocation(mProgram, "projectionMatrix");

  glUseProgram(mProgram);

  glUniform2f(offsetUniform, 0.5f, 0.5f);
  glUniformMatrix4fv(projMatUniform, 1, GL_FALSE, glm::value_ptr(makeProjectionMat(-1.0f, -3.0f, 1.0f)));

  OGL_CHECK_ERROR();
  glUseProgram(0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
}

void MatrixPerspective::render()
{
  glUseProgram(mProgram);

  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  // Geometry
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  // Colors
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(sizeof(ogl::perspectivePrism) / 2));
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
}
