#include "nodehierarchy.hpp"
#include "transform.hpp"
#include "shader.hpp"
#include "primitives/rectangle.hpp"
#include "matrixstack.hpp"
#include "hierarchy.hpp"
#include "sysutils.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstdio>

NodeHierarchy::NodeHierarchy(Window* window) :
  Tutorial(window),
  mProgram(0),
  mVao(0),
  mModelViewMatUnif(-1),
  mProjMatUnif(-1),
  mArmature("base", glm::vec3(3.0f, -5.0f, -40.0f), glm::vec3(0.0f, -45.0f, 0.0f))
{
  mProgram = ogl::makePorgram({
                                ogl::Shader(GL_VERTEX_SHADER, "shaders/modeltoclip.vert"),
                                ogl::Shader(GL_FRAGMENT_SHADER, "shaders/colored-triangle.frag")
                              });

  mModelViewMatUnif = glGetUniformLocation(mProgram, "modelToViewMatrix");
  mProjMatUnif = glGetUniformLocation(mProgram, "projectionMatrix");

  Node::transformUniform = mModelViewMatUnif;

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

  makeArmature();
  mWindow->addKeyCallback(onKeyPressed);
}

void NodeHierarchy::renderInternal()
{
  glClearDepth(1.0f);
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(mProgram);
  glBindVertexArray(mVao);

  mArmature.render(glm::mat4(1.0f));

  glBindVertexArray(0);
  glUseProgram(0);
}

void NodeHierarchy::framebufferSizeChanged(int w, int h)
{
  glUseProgram(mProgram);
  glUniformMatrix4fv(mProjMatUnif, 1, GL_FALSE, glm::value_ptr(ogl::makeProjectionMatFov(-1.0f, -100.0f, 45.0f, static_cast<float>(w)/h)));
  glUseProgram(0);

  glViewport(0, 0, w, h);
}

void NodeHierarchy::makeArmature()
{
  mArmature.addChild(Node("baseLeft", glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(2.0f, 2.0f, 6.0f)));
  mArmature.addChild(Node("baseRight", glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(2.0f, 2.0f, 6.0f)));

  Node& upperArm = mArmature.addChild(
        Node("upperArm", glm::vec3(0.0f), glm::vec3(-33.75f, 0.f, 0.f), glm::vec3(0.f, 0.f, 3.5f), glm::vec3(2.f, 2.f, 9.f))
      );
  Node& lowerArm = upperArm.addChild(
        Node("lowerArm", glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(146.25f, 0.f, 0.f), glm::vec3(0.f, 0.f, 2.5f), glm::vec3(1.5f, 1.5f, 5.f))
      );
  Node& wrist = lowerArm.addChild(
        Node("wrist", glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(67.5f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(2.0f, 2.0f, 2.0f))
      );

  Node& leftFinger = wrist.addChild(
        Node("leftFinger", glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.f, 180.0f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.5f, 0.5f, 2.0f))
      );
  (void) leftFinger.addChild(
        Node("leftLowerFinger", glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.f, -45.0f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.5f, 0.5f, 2.0f))
      );

  Node& rightFinger = wrist.addChild(
        Node("rightFinger", glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.f, -180.0f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.5f, 0.5f, 2.0f))
      );
  (void) rightFinger.addChild(
        Node("rightLowerFinger", glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.f, 45.0f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.5f, 0.5f, 2.0f))
      );
}

void NodeHierarchy::AdjBase(bool bIncrement)
{
  mArmature.angles.y += bIncrement ? Hierarchy::STANDARD_ANGLE_INCREMENT : -Hierarchy::STANDARD_ANGLE_INCREMENT;
  mArmature.angles.y = std::fmod(mArmature.angles.y, 360.0f);
}

void NodeHierarchy::AdjUpperArm(bool bIncrement)
{
  Node* upperArm = mArmature.find("upperArm");
  upperArm->angles.x += bIncrement ? Hierarchy::STANDARD_ANGLE_INCREMENT : -Hierarchy::STANDARD_ANGLE_INCREMENT;
  upperArm->angles.x = ogl::clamp(upperArm->angles.x, -90.0f, 0.0f);
}

void NodeHierarchy::AdjLowerArm(bool bIncrement)
{
  Node* lowerArm = mArmature.find("lowerArm");
  lowerArm->angles.x += bIncrement ? Hierarchy::STANDARD_ANGLE_INCREMENT : -Hierarchy::STANDARD_ANGLE_INCREMENT;
  lowerArm->angles.x = ogl::clamp(lowerArm->angles.x, 0.0f, 146.25f);
}

void NodeHierarchy::AdjWristPitch(bool bIncrement)
{
  Node* wrist = mArmature.find("wrist");
  wrist->angles.x += bIncrement ? Hierarchy::STANDARD_ANGLE_INCREMENT : -Hierarchy::STANDARD_ANGLE_INCREMENT;
  wrist->angles.x = ogl::clamp(wrist->angles.x, 0.0f, 90.0f);
}

void NodeHierarchy::AdjWristRoll(bool bIncrement)
{
  Node* wrist = mArmature.find("wrist");
  wrist->angles.z += bIncrement ? Hierarchy::STANDARD_ANGLE_INCREMENT : -Hierarchy::STANDARD_ANGLE_INCREMENT;
  wrist->angles.z = std::fmod(wrist->angles.z, 360.0f);
}

void NodeHierarchy::AdjFingerOpen(bool bIncrement)
{
  Node* leftFinger = mArmature.find("leftFinger");
  Node* rightFinger = mArmature.find("rightFinger");

  leftFinger->angles.y += bIncrement ? Hierarchy::SMALL_ANGLE_INCREMENT : -Hierarchy::SMALL_ANGLE_INCREMENT;
  leftFinger->angles.y = ogl::clamp(leftFinger->angles.y, 9.0f, 180.0f);
  rightFinger->angles.y = - leftFinger->angles.y;
}

void NodeHierarchy::WritePose()
{
  std::printf("angBase:\t%f\n", mArmature.angles.y);
  std::printf("upperArm->angles.x:\t%f\n", mArmature.find("upperArm")->angles.x);
  std::printf("lowerArm->angles.x:\t%f\n", mArmature.find("lowerArm")->angles.x);
  std::printf("wrist->angles.x:\t%f\n", mArmature.find("wrist")->angles.x);
  std::printf("angWristRoll:\t%f\n", mArmature.find("wrist")->angles.z);
  std::printf("fingerOpen:\t%f\n", mArmature.find("leftFinger")->angles.y);
  std::printf("\n");
}

void NodeHierarchy::onKeyPressed(Window* win, int key, int /*scancode*/, Window::Action action, short /*keyMod*/)
{
  if (action == Window::RELEASE) {
    return;
  }

  NodeHierarchy* armature = static_cast<NodeHierarchy*>(win->getTutorial());

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
