#ifndef NODEHIERARCHY_HPP
#define NODEHIERARCHY_HPP

#include "tutorial.hpp"
#include "node.hpp"
#include "window.hpp"
#include <glad/glad.h>

class NodeHierarchy : public Tutorial
{
public:
  NodeHierarchy(Window* window);

private:
  static void onKeyPressed(Window* win, int key, int, Window::KeyAction action);

  void renderInternal() override;
  void framebufferSizeChanged(int w, int h) override;

  void makeArmature();

  void AdjBase(bool bIncrement);
  void AdjUpperArm(bool bIncrement);
  void AdjLowerArm(bool bIncrement);
  void AdjWristPitch(bool bIncrement);
  void AdjWristRoll(bool bIncrement);
  void AdjFingerOpen(bool bIncrement);
  void WritePose();

private:
  // GL objects
  GLuint mProgram;
  GLuint mVao;

  // Shader uniforms
  GLint mModelViewMatUnif;
  GLint mProjMatUnif;

  // Armature
  Node mArmature;
};

#endif // NODEHIERARCHY_HPP
