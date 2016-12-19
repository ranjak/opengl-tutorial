#include "translation.hpp"
#include "shader.hpp"
#include "transform.hpp"
#include "primitives/pointyshape.hpp"
#include "system.hpp"
#include "util.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace
{
glm::mat4 StationaryOffset(float /*fElapsedTime*/)
{
  return ogl::translate(glm::vec3(0.0f, 0.0f, -20.0f));
}

glm::mat4 OvalOffset(float fElapsedTime)
{
  const float fLoopDuration = 3.0f;
  const float fScale = 360.0f / fLoopDuration;

  float fCurrTimeThroughLoop = std::fmod(fElapsedTime, fLoopDuration);

  glm::mat4 transform = glm::mat4(ogl::rotateZ(fCurrTimeThroughLoop * fScale));
  transform = transform * ogl::translate(glm::vec3(4.0f, 6.0f, -20.0f));
  // Keep the object orientation constant
  transform = transform * glm::mat4(ogl::rotateZ(-fCurrTimeThroughLoop * fScale));

  return transform;
}

glm::mat4 BottomCircleOffset(float fElapsedTime)
{
  const float fLoopDuration = 12.0f;
  const float fScale = 360.0f / fLoopDuration;

  float fCurrTimeThroughLoop = std::fmod(fElapsedTime, fLoopDuration);

  glm::mat4 transform = ogl::translate(glm::vec3(0.0f, 0.0, -20.0f));
  transform = transform * glm::mat4(ogl::rotateY(fCurrTimeThroughLoop * fScale));
  transform = transform * ogl::translate(glm::vec3(5.0f, -3.5f, 0.0f));
  // Keep the object orientation constant
  transform = transform * glm::mat4(ogl::rotateY(-fCurrTimeThroughLoop * fScale));

  return transform;
}

} // namespace

Translation::Translation(Window* window) :
  Tutorial(window),
  mProgram(0),
  mVao(0),
  mModelViewMatUnif(-1),
  mProjMatUnif(-1),
  mPositions{ { StationaryOffset }, { OvalOffset }, { BottomCircleOffset } }
{
  mProgram = ogl::makePorgram({
                                ogl::Shader(GL_VERTEX_SHADER, "shaders/modeltoclip.vert"),
                                ogl::Shader(GL_FRAGMENT_SHADER, "shaders/colored-triangle.frag")
                              });

  mModelViewMatUnif = glGetUniformLocation(mProgram, "modelToViewMatrix");
  mProjMatUnif = glGetUniformLocation(mProgram, "projectionMatrix");

  // GL buffers and attrib arrays setup
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pointyshape::vertexData), pointyshape::vertexData, GL_STATIC_DRAW);

  GLuint ibo = 0;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pointyshape::indexData), pointyshape::indexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  int colorOffset = sizeof(float) * 3 * pointyshape::numVertices;

  glGenVertexArrays(1, &mVao);
  glBindVertexArray(mVao);
  // Geometry
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // Colors: in the same buffer, but after every vertex
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(colorOffset));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  OGL_CHECK_ERROR();

  // GL Settings
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);
}

void Translation::renderInternal()
{
  glClearDepth(1.0f);
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(mProgram);
  glBindVertexArray(mVao);

  // Update object positions
  for (Instance& pos : mPositions) {
    glUniformMatrix4fv(mModelViewMatUnif, 1, GL_FALSE, glm::value_ptr(pos.ConstructMatrix(ogl::seconds(System::now()).count())));
    OGL_CHECK_ERROR();

    glDrawElements(GL_TRIANGLES, pointyshape::numIndices, GL_UNSIGNED_SHORT, 0);
    OGL_CHECK_ERROR();
  }

  glBindVertexArray(0);
  glUseProgram(0);
}

void Translation::framebufferSizeChanged(int w, int h)
{
  glUseProgram(mProgram);
  glUniformMatrix4fv(mProjMatUnif, 1, GL_FALSE, glm::value_ptr(ogl::makeProjectionMatFov(-1.0f, -45.0f, 45.0f, static_cast<float>(w)/h)));
  glUseProgram(0);

  glViewport(0, 0, w, h);
}

glm::mat4 Instance::ConstructMatrix(float fElapsedTime)
{
  return CalcOffset(fElapsedTime);
}
