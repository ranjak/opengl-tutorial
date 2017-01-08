#include "hierarchy.hpp"
#include "primitives/rectangle.hpp"
#include "shader.hpp"
#include "matrixstack.hpp"
#include "sysutils.hpp"
#include "log.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

const float Hierarchy::STANDARD_ANGLE_INCREMENT = 11.25f;
const float Hierarchy::SMALL_ANGLE_INCREMENT = 9.0f;

Hierarchy::Hierarchy(Window* window) :
  Tutorial(window),
  mProgram(0),
  mVao(0),
  mModelViewMatUnif(-1),
  mProjMatUnif(-1)
  // Armature
  , posBase(glm::vec3(3.0f, -5.0f, -40.0f))
  , angBase(-45.0f)
  , posBaseLeft(glm::vec3(2.0f, 0.0f, 0.0f))
  , posBaseRight(glm::vec3(-2.0f, 0.0f, 0.0f))
  , scaleBaseZ(3.0f)
  , angUpperArm(-33.75f)
  , sizeUpperArm(9.0f)
  , posLowerArm(glm::vec3(0.0f, 0.0f, 8.0f))
  , angLowerArm(146.25f)
  , lenLowerArm(5.0f)
  , widthLowerArm(1.5f)
  , posWrist(glm::vec3(0.0f, 0.0f, 5.0f))
  , angWristRoll(0.0f)
  , angWristPitch(67.5f)
  , lenWrist(2.0f)
  , widthWrist(2.0f)
  , posLeftFinger(glm::vec3(1.0f, 0.0f, 1.0f))
  , posRightFinger(glm::vec3(-1.0f, 0.0f, 1.0f))
  , angFingerOpen(180.0f)
  , lenFinger(2.0f)
  , widthFinger(0.5f)
  , angLowerFinger(45.0f)
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle::vertexData), rectangle::vertexData, GL_STATIC_DRAW);

  GLuint ibo = 0;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle::indexData), rectangle::indexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  int colorOffset = sizeof(float) * 3 * rectangle::numVertices;

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

  // GL Settings
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);

  mWindow->addKeyCallback(onKeyPressed);
}

void Hierarchy::framebufferSizeChanged(int w, int h)
{
  glUseProgram(mProgram);
  glUniformMatrix4fv(mProjMatUnif, 1, GL_FALSE, glm::value_ptr(ogl::makeProjectionMatFov(-1.0f, -100.0f, 45.0f, static_cast<float>(w)/h)));
  glUseProgram(0);

  glViewport(0, 0, w, h);
}

void Hierarchy::Draw()
{
  MatrixStack modelToCameraStack;

  glUseProgram(mProgram);
  glBindVertexArray(mVao);

  modelToCameraStack.translate(posBase);
  modelToCameraStack.rotateY(angBase);

  //Draw left base.
  {
    modelToCameraStack.push();
    modelToCameraStack.translate(posBaseLeft);
    modelToCameraStack.scale(glm::vec3(1.0f, 1.0f, scaleBaseZ));
    glUniformMatrix4fv(mModelViewMatUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
    glDrawElements(GL_TRIANGLES, rectangle::numIndices, GL_UNSIGNED_SHORT, 0);
    modelToCameraStack.pop();
  }

  //Draw right base.
  {
    modelToCameraStack.push();
    modelToCameraStack.translate(posBaseRight);
    modelToCameraStack.scale(glm::vec3(1.0f, 1.0f, scaleBaseZ));
    glUniformMatrix4fv(mModelViewMatUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
    glDrawElements(GL_TRIANGLES, rectangle::numIndices, GL_UNSIGNED_SHORT, 0);
    modelToCameraStack.pop();
  }

  //Draw main arm.
  DrawUpperArm(modelToCameraStack);

  glBindVertexArray(0);
  glUseProgram(0);
}

void Hierarchy::AdjBase(bool bIncrement)
{
  angBase += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
  angBase = fmodf(angBase, 360.0f);
}

void Hierarchy::AdjUpperArm(bool bIncrement)
{
  angUpperArm += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
  angUpperArm = ogl::clamp(angUpperArm, -90.0f, 0.0f);
}

void Hierarchy::AdjLowerArm(bool bIncrement)
{
  angLowerArm += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
  angLowerArm = ogl::clamp(angLowerArm, 0.0f, 146.25f);
}

void Hierarchy::AdjWristPitch(bool bIncrement)
{
  angWristPitch += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
  angWristPitch = ogl::clamp(angWristPitch, 0.0f, 90.0f);
}

void Hierarchy::AdjWristRoll(bool bIncrement)
{
  angWristRoll += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
  angWristRoll = fmodf(angWristRoll, 360.0f);
}

void Hierarchy::AdjFingerOpen(bool bIncrement)
{
  angFingerOpen += bIncrement ? SMALL_ANGLE_INCREMENT : -SMALL_ANGLE_INCREMENT;
  angFingerOpen = ogl::clamp(angFingerOpen, 9.0f, 180.0f);
}

void Hierarchy::WritePose()
{
  std::printf("angBase:\t%f\n", angBase);
  std::printf("angUpperArm:\t%f\n", angUpperArm);
  std::printf("angLowerArm:\t%f\n", angLowerArm);
  std::printf("angWristPitch:\t%f\n", angWristPitch);
  std::printf("angWristRoll:\t%f\n", angWristRoll);
  std::printf("angFingerOpen:\t%f\n", angFingerOpen);
  std::printf("\n");
}

void Hierarchy::DrawFingers(MatrixStack& modelToCameraStack)
{
  //Draw left finger
  modelToCameraStack.push();
  modelToCameraStack.translate(posLeftFinger);
  modelToCameraStack.rotateY(angFingerOpen);

  modelToCameraStack.push();
  modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
  modelToCameraStack.scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
  glUniformMatrix4fv(mModelViewMatUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
  glDrawElements(GL_TRIANGLES, rectangle::numIndices, GL_UNSIGNED_SHORT, 0);
  modelToCameraStack.pop();

  {
    //Draw left lower finger
    modelToCameraStack.push();
    modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, lenFinger));
    modelToCameraStack.rotateY(-angLowerFinger);

    modelToCameraStack.push();
    modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
    modelToCameraStack.scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
    glUniformMatrix4fv(mModelViewMatUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
    glDrawElements(GL_TRIANGLES, rectangle::numIndices, GL_UNSIGNED_SHORT, 0);
    modelToCameraStack.pop();

    modelToCameraStack.pop();
  }

  modelToCameraStack.pop();

  //Draw right finger
  modelToCameraStack.push();
  modelToCameraStack.translate(posRightFinger);
  modelToCameraStack.rotateY(-angFingerOpen);

  modelToCameraStack.push();
  modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
  modelToCameraStack.scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
  glUniformMatrix4fv(mModelViewMatUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
  glDrawElements(GL_TRIANGLES, rectangle::numIndices, GL_UNSIGNED_SHORT, 0);
  modelToCameraStack.pop();

  {
    //Draw right lower finger
    modelToCameraStack.push();
    modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, lenFinger));
    modelToCameraStack.rotateY(angLowerFinger);

    modelToCameraStack.push();
    modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
    modelToCameraStack.scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
    glUniformMatrix4fv(mModelViewMatUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
    glDrawElements(GL_TRIANGLES, rectangle::numIndices, GL_UNSIGNED_SHORT, 0);
    modelToCameraStack.pop();

    modelToCameraStack.pop();
  }

  modelToCameraStack.pop();
}

void Hierarchy::DrawWrist(MatrixStack& modelToCameraStack)
{
  modelToCameraStack.push();
  modelToCameraStack.translate(posWrist);
  modelToCameraStack.rotateZ(angWristRoll);
  modelToCameraStack.rotateX(angWristPitch);

  modelToCameraStack.push();
  modelToCameraStack.scale(glm::vec3(widthWrist / 2.0f, widthWrist/ 2.0f, lenWrist / 2.0f));
  glUniformMatrix4fv(mModelViewMatUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
  glDrawElements(GL_TRIANGLES, rectangle::numIndices, GL_UNSIGNED_SHORT, 0);
  modelToCameraStack.pop();

  DrawFingers(modelToCameraStack);

  modelToCameraStack.pop();
}

void Hierarchy::DrawLowerArm(MatrixStack& modelToCameraStack)
{
  modelToCameraStack.push();
  modelToCameraStack.translate(posLowerArm);
  modelToCameraStack.rotateX(angLowerArm);

  modelToCameraStack.push();
  modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, lenLowerArm / 2.0f));
  modelToCameraStack.scale(glm::vec3(widthLowerArm / 2.0f, widthLowerArm / 2.0f, lenLowerArm / 2.0f));
  glUniformMatrix4fv(mModelViewMatUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
  glDrawElements(GL_TRIANGLES, rectangle::numIndices, GL_UNSIGNED_SHORT, 0);
  modelToCameraStack.pop();

  DrawWrist(modelToCameraStack);

  modelToCameraStack.pop();
}

void Hierarchy::DrawUpperArm(MatrixStack& modelToCameraStack)
{
  modelToCameraStack.push();
  modelToCameraStack.rotateX(angUpperArm);

  {
    modelToCameraStack.push();
    modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, (sizeUpperArm / 2.0f) - 1.0f));
    modelToCameraStack.scale(glm::vec3(1.0f, 1.0f, sizeUpperArm / 2.0f));
    glUniformMatrix4fv(mModelViewMatUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
    glDrawElements(GL_TRIANGLES, rectangle::numIndices, GL_UNSIGNED_SHORT, 0);
    modelToCameraStack.pop();
  }

  DrawLowerArm(modelToCameraStack);

  modelToCameraStack.pop();
}

void Hierarchy::onKeyPressed(Window* win, int key, int /*scancode*/, Window::Action action, short /*keyMod*/)
{
  if (action == Window::RELEASE) {
    return;
  }

  Hierarchy* armature = static_cast<Hierarchy*>(win->getTutorial());

  switch (key)
  {
  case 'A': armature->AdjBase(true); break;
  case 'D': armature->AdjBase(false); break;
  case 'W': armature->AdjUpperArm(false); break;
  case 'S': armature->AdjUpperArm(true); break;
  case 'R': armature->AdjLowerArm(false); break;
  case 'F': armature->AdjLowerArm(true); break;
  case 'T': armature->AdjWristPitch(false); break;
  case 'G': armature->AdjWristPitch(true); break;
  case 'Z': armature->AdjWristRoll(true); break;
  case 'C': armature->AdjWristRoll(false); break;
  case 'Q': armature->AdjFingerOpen(true); break;
  case 'E': armature->AdjFingerOpen(false); break;
  case 32: armature->WritePose(); break;
  }
}

void Hierarchy::renderInternal()
{
  glClearDepth(1.0f);
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Draw();
}
