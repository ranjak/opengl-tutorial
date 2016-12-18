#ifndef HIERARCHY_HPP
#define HIERARCHY_HPP

#include "tutorial.hpp"
#include "window.hpp"
#include <glad/glad.h>
#include <glm/vec3.hpp>

class MatrixStack;

class Hierarchy : public Tutorial
{
public:
  Hierarchy(Window* window);

private:
  void renderInternal() override;
  void framebufferSizeChanged(int w, int h) override;

  // Armature functions
  static const float STANDARD_ANGLE_INCREMENT;
  static const float SMALL_ANGLE_INCREMENT;

  static void onKeyPressed(Window* win, int key, int, Window::KeyAction action);

  void AdjBase(bool bIncrement);
  void AdjUpperArm(bool bIncrement);
  void AdjLowerArm(bool bIncrement);
  void AdjWristPitch(bool bIncrement);
  void AdjWristRoll(bool bIncrement);
  void AdjFingerOpen(bool bIncrement);
  void WritePose();

  void Draw();
  void DrawFingers(MatrixStack &modelToCameraStack);
  void DrawWrist(MatrixStack &modelToCameraStack);
  void DrawLowerArm(MatrixStack &modelToCameraStack);
  void DrawUpperArm(MatrixStack &modelToCameraStack);

private:
  // GL objects
  GLuint mProgram;
  GLuint mVao;

  // Shader uniforms
  GLint mModelViewMatUnif;
  GLint mProjMatUnif;

  // Armature data
  glm::vec3		posBase;
  float			angBase;

  glm::vec3		posBaseLeft, posBaseRight;
  float			scaleBaseZ;

  float			angUpperArm;
  float			sizeUpperArm;

  glm::vec3		posLowerArm;
  float			angLowerArm;
  float			lenLowerArm;
  float			widthLowerArm;

  glm::vec3		posWrist;
  float			angWristRoll;
  float			angWristPitch;
  float			lenWrist;
  float			widthWrist;

  glm::vec3		posLeftFinger, posRightFinger;
  float			angFingerOpen;
  float			lenFinger;
  float			widthFinger;
  float			angLowerFinger;
};

#endif // HIERARCHY_HPP
