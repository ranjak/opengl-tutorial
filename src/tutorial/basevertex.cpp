#include "basevertex.hpp"
#include "primitives/indexedwedges.hpp"
#include "transform.hpp"
#include "shader.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

BaseVertex::BaseVertex(Window* window) :
  Tutorial(window),
  mProgram(0),
  mVao(0),
  mProjMatUniform(-1),
  mOffsetUniform(-1)
{
  initShaders();
  initObjects();

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
}

void BaseVertex::renderInternal()
{
  glUseProgram(mProgram);
  glBindVertexArray(mVao);

  // Draw wedge 1
  glUniform3f(mOffsetUniform, 0.0f, 0.0f, 0.0f);
  glDrawElements(GL_TRIANGLES, ogl::wedgesNumElements, GL_UNSIGNED_SHORT, 0);

  // Draw wedge 2
  glUniform3f(mOffsetUniform, 0.0f, 0.0f, -1.0f);
  glDrawElementsBaseVertex(GL_TRIANGLES, ogl::wedgesNumElements, GL_UNSIGNED_SHORT, 0, ogl::wedgesNumVertices / 2);

  glBindVertexArray(0);
  glUseProgram(0);
}

void BaseVertex::framebufferSizeChanged(int w, int h)
{
  glUseProgram(mProgram);
  glUniformMatrix4fv(mProjMatUniform, 1, GL_FALSE, glm::value_ptr(ogl::makeProjectionMat(-1.0f, -3.0f, 1.0, static_cast<float>(w)/h)));
  glUseProgram(0);

  glViewport(0, 0, w, h);
}

void BaseVertex::initShaders()
{
  mProgram = ogl::makePorgram({
                                ogl::Shader(GL_VERTEX_SHADER, "shaders/matrixperspective-vec3offset.vert"),
                                ogl::Shader(GL_FRAGMENT_SHADER, "shaders/colored-triangle.frag")
                              });
  mOffsetUniform = glGetUniformLocation(mProgram, "offset");
  mProjMatUniform = glGetUniformLocation(mProgram, "projectionMatrix");
}

void BaseVertex::initObjects()
{
  // Get all vertex data in a single buffer
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ogl::wedgesVertexData), ogl::wedgesVertexData, GL_STATIC_DRAW);

  GLuint ibo = 0;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ogl::wedgesIndexData), ogl::wedgesIndexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Use a single VAO for both wedges
  int wedgesColorOffset = sizeof(float) * 3 * ogl::wedgesNumVertices;

  glGenVertexArrays(1, &mVao);
  glBindVertexArray(mVao);
  // Geometry
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // Colors: in the same buffer, but after every vertex
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(wedgesColorOffset));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
