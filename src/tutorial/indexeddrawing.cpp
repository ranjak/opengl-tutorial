#include "indexeddrawing.hpp"
#include "transform.hpp"
#include "primitives/indexedwedges.hpp"
#include "shader.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>


IndexedDrawing::IndexedDrawing(Window* window) :
  Tutorial(window),
  mProgram(0),
  mVao(0),
  mVao2(0),
  mProjMatUniform(-1),
  mOffsetUniform(-1)
{
  initShaders();
  initObjects();

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
}

void IndexedDrawing::renderInternal()
{
  glUseProgram(mProgram);

  // Draw wedge 1
  glUniform3f(mOffsetUniform, 0.0f, 0.0f, 0.0f);
  glBindVertexArray(mVao);
  glDrawElements(GL_TRIANGLES, ogl::wedgesNumElements, GL_UNSIGNED_SHORT, 0);

  // Draw wedge 2
  glUniform3f(mOffsetUniform, 0.0f, 0.0f, -1.0f);
  glBindVertexArray(mVao2);
  glDrawElements(GL_TRIANGLES, ogl::wedgesNumElements, GL_UNSIGNED_SHORT, 0);

  glBindVertexArray(0);
  glUseProgram(0);
}

void IndexedDrawing::framebufferSizeChanged(int w, int h)
{
  glUseProgram(mProgram);
  glUniformMatrix4fv(mProjMatUniform, 1, GL_FALSE, glm::value_ptr(ogl::makeProjectionMat(-1.0f, -3.0f, 1.0, static_cast<float>(w)/h)));
  glUseProgram(0);

  glViewport(0, 0, w, h);
}

void IndexedDrawing::initShaders()
{
  mProgram = ogl::makePorgram({
                                ogl::Shader(GL_VERTEX_SHADER, "shaders/matrixperspective-vec3offset.vert"),
                                ogl::Shader(GL_FRAGMENT_SHADER, "shaders/colored-triangle.frag")
                              });
  mOffsetUniform = glGetUniformLocation(mProgram, "offset");
  mProjMatUniform = glGetUniformLocation(mProgram, "projectionMatrix");
}

void IndexedDrawing::initObjects()
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

  // First wedge
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

  // Second wedge
  int secondWedgeVertexOffset = wedgesColorOffset / 2;
  wedgesColorOffset += sizeof(float) * 4 * ogl::wedgesNumVertices / 2;

  glGenVertexArrays(1, &mVao2);
  glBindVertexArray(mVao2);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(secondWedgeVertexOffset));
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(wedgesColorOffset));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindVertexArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
