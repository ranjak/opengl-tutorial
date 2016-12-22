#include "worldscene.hpp"
#include "shader.hpp"
#include "forest.hpp"
#include "transform.hpp"
#include <glutil/MatrixStack.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <cstddef>
#include <cmath>

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

namespace
{

float g_fzNear = 1.0f;
float g_fzFar = 1000.0f;

// Parthenon constants
const float g_fColumnBaseHeight = 0.25f;
const float g_fParthenonWidth = 14.0f;
const float g_fParthenonLength = 20.0f;
const float g_fParthenonColumnHeight = 5.0f;
const float g_fParthenonBaseHeight = 1.0f;
const float g_fParthenonTopHeight = 2.0f;

ProgramData LoadProgram(const std::string& strVertexShader, const std::string& strFragmentShader)
{
  std::initializer_list<ogl::Shader> shaderList {
    ogl::Shader(GL_VERTEX_SHADER, strVertexShader),
        ogl::Shader(GL_FRAGMENT_SHADER, strFragmentShader)
  };

  ProgramData data;
  data.theProgram = ogl::makePorgram(shaderList);
  data.modelToWorldMatrixUnif = glGetUniformLocation(data.theProgram, "modelToWorldMatrix");
  data.worldToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "worldToCameraMatrix");
  data.cameraToClipMatrixUnif = glGetUniformLocation(data.theProgram, "cameraToClipMatrix");
  data.baseColorUnif = glGetUniformLocation(data.theProgram, "baseColor");

  return data;
}

glm::mat4 CalcLookAtMatrix(const glm::vec3& cameraPt, const glm::vec3& lookPt, const glm::vec3& upPt)
{
  glm::vec3 lookDir = glm::normalize(lookPt - cameraPt);
  glm::vec3 upDir = glm::normalize(upPt);

  glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
  glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

  glm::mat4 rotMat(1.0f);
  rotMat[0] = glm::vec4(rightDir, 0.0f);
  rotMat[1] = glm::vec4(perpUpDir, 0.0f);
  rotMat[2] = glm::vec4(-lookDir, 0.0f);

  rotMat = glm::transpose(rotMat);

  glm::mat4 transMat(1.0f);
  transMat[3] = glm::vec4(-cameraPt, 1.0f);

  return rotMat * transMat;
}

} // namespace

WorldScene::WorldScene(Window* window) :
  Tutorial(window),
  mUniformColor(LoadProgram("shaders/world/PosOnlyWorldTransform.vert", "shaders/world/ColorUniform.frag")),
  mObjectColor(LoadProgram("shaders/world/PosColorWorldTransform.vert", "shaders/world/ColorPassthrough.frag")),
  mUniformColorTint(LoadProgram("shaders/world/PosColorWorldTransform.vert", "shaders/world/ColorMultUniform.frag")),
  mConeMesh("assets/UnitConeTint.xml"),
  mCylinderMesh("assets/UnitCylinderTint.xml"),
  mCubeTintMesh("assets/UnitCubeTint.xml"),
  mCubeColorMesh("assets/UnitCubeColor.xml"),
  mPlaneMesh("assets/UnitPlane.xml"),
  mDrawLookatPoint(false),
  mCamTarget(0.0f, 0.4f, 0.0f),
  mSphereCamRelPos(67.5f, -46.0f, 150.0f)
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);
  glEnable(GL_DEPTH_CLAMP);

  mWindow->addKeyCallback(onKeyPressed);
}

void WorldScene::onKeyPressed(Window* win, int key, int, Window::KeyAction act, short mods)
{
  if (act == Window::KEYUP) {
    return;
  }

  WorldScene* thisScene = static_cast<WorldScene*>(win->getTutorial());

  // Controls are more precise when holding Shift
  float multiplier = (mods & Window::MOD_SHIFT) ? 1.0f : 10.0f;

  glm::vec3& camTarget = thisScene->mCamTarget;
  glm::vec3& sphereCamRelPos = thisScene->mSphereCamRelPos;

  switch (key) {
  case 'W': camTarget.z -= 0.4f * multiplier; break;
  case 'S': camTarget.z += 0.4f * multiplier; break;
  case 'D': camTarget.x += 0.4f * multiplier; break;
  case 'A': camTarget.x -= 0.4f * multiplier; break;
  case 'E': camTarget.y -= 0.4f * multiplier; break;
  case 'Q': camTarget.y += 0.4f * multiplier; break;

  case 'I': sphereCamRelPos.y -= 1.125f * multiplier; break;
  case 'K': sphereCamRelPos.y += 1.125f * multiplier; break;
  case 'J': sphereCamRelPos.x -= 1.125f * multiplier; break;
  case 'L': sphereCamRelPos.x += 1.125f * multiplier; break;
  case 'O': sphereCamRelPos.z -= 0.5f * multiplier; break;
  case 'U': sphereCamRelPos.z += 0.5f * multiplier; break;

  case 32:
    thisScene->mDrawLookatPoint = !thisScene->mDrawLookatPoint;
    printf("Target: %f, %f, %f\n", camTarget.x, camTarget.y, camTarget.z);
    printf("Position: %f, %f, %f\n", sphereCamRelPos.x, sphereCamRelPos.y, sphereCamRelPos.z);
    break;
  }

  sphereCamRelPos.y = glm::clamp(sphereCamRelPos.y, -78.75f, -1.0f);
  camTarget.y = camTarget.y > 0.0f ? camTarget.y : 0.0f;
  sphereCamRelPos.z = sphereCamRelPos.z > 5.0f ? sphereCamRelPos.z : 5.0f;
}

void WorldScene::DrawTree(glutil::MatrixStack& modelMatrix, float fTrunkHeight, float fConeHeight)
{
  //Draw trunk.
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Scale(glm::vec3(1.0f, fTrunkHeight, 1.0f));
    modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

    glUseProgram(mUniformColorTint.theProgram);
    glUniformMatrix4fv(mUniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniform4f(mUniformColorTint.baseColorUnif, 0.694f, 0.4f, 0.106f, 1.0f);
    mCylinderMesh.Render();
    glUseProgram(0);
  }

  //Draw the treetop
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Translate(glm::vec3(0.0f, fTrunkHeight, 0.0f));
    modelMatrix.Scale(glm::vec3(3.0f, fConeHeight, 3.0f));

    glUseProgram(mUniformColorTint.theProgram);
    glUniformMatrix4fv(mUniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniform4f(mUniformColorTint.baseColorUnif, 0.0f, 1.0f, 0.0f, 1.0f);
    mConeMesh.Render();
    glUseProgram(0);
  }
}

//Columns are 1x1 in the X/Z, and fHieght units in the Y.
void WorldScene::DrawColumn(glutil::MatrixStack& modelMatrix, float fHeight)
{
  //Draw the bottom of the column.
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Scale(glm::vec3(1.0f, g_fColumnBaseHeight, 1.0f));
    modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

    glUseProgram(mUniformColorTint.theProgram);
    glUniformMatrix4fv(mUniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniform4f(mUniformColorTint.baseColorUnif, 1.0f, 1.0f, 1.0f, 1.0f);
    mCubeTintMesh.Render();
    glUseProgram(0);
  }

  //Draw the top of the column.
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Translate(glm::vec3(0.0f, fHeight - g_fColumnBaseHeight, 0.0f));
    modelMatrix.Scale(glm::vec3(1.0f, g_fColumnBaseHeight, 1.0f));
    modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

    glUseProgram(mUniformColorTint.theProgram);
    glUniformMatrix4fv(mUniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniform4f(mUniformColorTint.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
    mCubeTintMesh.Render();
    glUseProgram(0);
  }

  //Draw the main column.
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Translate(glm::vec3(0.0f, g_fColumnBaseHeight, 0.0f));
    modelMatrix.Scale(glm::vec3(0.8f, fHeight - (g_fColumnBaseHeight * 2.0f), 0.8f));
    modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

    glUseProgram(mUniformColorTint.theProgram);
    glUniformMatrix4fv(mUniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniform4f(mUniformColorTint.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
    mCylinderMesh.Render();
    glUseProgram(0);
  }
}

void WorldScene::DrawParthenon(glutil::MatrixStack& modelMatrix)
{
  //Draw base.
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Scale(glm::vec3(g_fParthenonWidth, g_fParthenonBaseHeight, g_fParthenonLength));
    modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

    glUseProgram(mUniformColorTint.theProgram);
    glUniformMatrix4fv(mUniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniform4f(mUniformColorTint.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
    mCubeTintMesh.Render();
    glUseProgram(0);
  }

  //Draw top.
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Translate(glm::vec3(0.0f, g_fParthenonColumnHeight + g_fParthenonBaseHeight, 0.0f));
    modelMatrix.Scale(glm::vec3(g_fParthenonWidth, g_fParthenonTopHeight, g_fParthenonLength));
    modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

    glUseProgram(mUniformColorTint.theProgram);
    glUniformMatrix4fv(mUniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniform4f(mUniformColorTint.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
    mCubeTintMesh.Render();
    glUseProgram(0);
  }

  //Draw columns.
  const float fFrontZVal = (g_fParthenonLength / 2.0f) - 1.0f;
  const float fRightXVal = (g_fParthenonWidth / 2.0f) - 1.0f;

  for(int iColumnNum = 0; iColumnNum < int(g_fParthenonWidth / 2.0f); iColumnNum++)
  {
    {
      glutil::PushStack push(modelMatrix);
      modelMatrix.Translate(glm::vec3((2.0f * iColumnNum) - (g_fParthenonWidth / 2.0f) + 1.0f,
                                      g_fParthenonBaseHeight, fFrontZVal));

      DrawColumn(modelMatrix, g_fParthenonColumnHeight);
    }
    {
      glutil::PushStack push(modelMatrix);
      modelMatrix.Translate(glm::vec3((2.0f * iColumnNum) - (g_fParthenonWidth / 2.0f) + 1.0f,
                                      g_fParthenonBaseHeight, -fFrontZVal));

      DrawColumn(modelMatrix, g_fParthenonColumnHeight);
    }
  }

  //Don't draw the first or last columns, since they've been drawn already.
  for(int iColumnNum = 1; iColumnNum < int((g_fParthenonLength - 2.0f) / 2.0f); iColumnNum++)
  {
    {
      glutil::PushStack push(modelMatrix);
      modelMatrix.Translate(glm::vec3(fRightXVal,
                                      g_fParthenonBaseHeight, (2.0f * iColumnNum) - (g_fParthenonLength / 2.0f) + 1.0f));

      DrawColumn(modelMatrix, g_fParthenonColumnHeight);
    }
    {
      glutil::PushStack push(modelMatrix);
      modelMatrix.Translate(glm::vec3(-fRightXVal,
                                      g_fParthenonBaseHeight, (2.0f * iColumnNum) - (g_fParthenonLength / 2.0f) + 1.0f));

      DrawColumn(modelMatrix, g_fParthenonColumnHeight);
    }
  }

  //Draw interior.
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Translate(glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix.Scale(glm::vec3(g_fParthenonWidth - 6.0f, g_fParthenonColumnHeight,
                                g_fParthenonLength - 6.0f));
    modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

    glUseProgram(mObjectColor.theProgram);
    glUniformMatrix4fv(mObjectColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    mCubeColorMesh.Render();
    glUseProgram(0);
  }

  //Draw headpiece.
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Translate(glm::vec3(
                            0.0f,
                            g_fParthenonColumnHeight + g_fParthenonBaseHeight + (g_fParthenonTopHeight / 2.0f),
                            g_fParthenonLength / 2.0f));
    modelMatrix.RotateX(-135.0f);
    modelMatrix.RotateY(45.0f);

    glUseProgram(mObjectColor.theProgram);
    glUniformMatrix4fv(mObjectColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    mCubeColorMesh.Render();
    glUseProgram(0);
  }
}

void WorldScene::DrawForest(glutil::MatrixStack& modelMatrix)
{
  for(std::size_t iTree = 0; iTree < ARRAY_COUNT(g_forest); iTree++)
  {
    const TreeData &currTree = g_forest[iTree];

    glutil::PushStack push(modelMatrix);
    modelMatrix.Translate(glm::vec3(currTree.fXPos, 0.0f, currTree.fZPos));
    DrawTree(modelMatrix, currTree.fTrunkHeight, currTree.fConeHeight);
  }
}

glm::vec3 WorldScene::ResolveCamPosition()
{
  float phi = ogl::degToRadf(mSphereCamRelPos.x);
  float theta = ogl::degToRadf(mSphereCamRelPos.y + 90.0f);

  float fSinTheta = std::sin(theta);
  float fCosTheta = std::cos(theta);
  float fCosPhi = std::cos(phi);
  float fSinPhi = std::sin(phi);

  glm::vec3 dirToCamera(fSinTheta * fCosPhi, fCosTheta, fSinTheta * fSinPhi);
  return (dirToCamera * mSphereCamRelPos.z) + mCamTarget;
}

void WorldScene::renderInternal()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const glm::vec3 &camPos = ResolveCamPosition();

  glutil::MatrixStack camMatrix;
  camMatrix.SetMatrix(CalcLookAtMatrix(camPos, mCamTarget, glm::vec3(0.0f, 1.0f, 0.0f)));

  glUseProgram(mUniformColor.theProgram);
  glUniformMatrix4fv(mUniformColor.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
  glUseProgram(mObjectColor.theProgram);
  glUniformMatrix4fv(mObjectColor.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
  glUseProgram(mUniformColorTint.theProgram);
  glUniformMatrix4fv(mUniformColorTint.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
  glUseProgram(0);

  glutil::MatrixStack modelMatrix;

  //Render the ground plane.
  {
    glutil::PushStack push(modelMatrix);

    modelMatrix.Scale(glm::vec3(100.0f, 1.0f, 100.0f));

    glUseProgram(mUniformColor.theProgram);
    glUniformMatrix4fv(mUniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniform4f(mUniformColor.baseColorUnif, 0.302f, 0.416f, 0.0589f, 1.0f);
    mPlaneMesh.Render();
    glUseProgram(0);
  }

  //Draw the trees
  DrawForest(modelMatrix);

  //Draw the building.
  {
    glutil::PushStack push(modelMatrix);
    modelMatrix.Translate(glm::vec3(20.0f, 0.0f, -10.0f));

    DrawParthenon(modelMatrix);
  }

  if(mDrawLookatPoint)
  {
    glDisable(GL_DEPTH_TEST);
    glm::mat4 idenity(1.0f);

    glutil::PushStack push(modelMatrix);

    glm::vec3 cameraAimVec = mCamTarget - camPos;
    modelMatrix.Translate(0.0f, 0.0, -glm::length(cameraAimVec));
    modelMatrix.Scale(1.0f, 1.0f, 1.0f);

    glUseProgram(mObjectColor.theProgram);
    glUniformMatrix4fv(mObjectColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
    glUniformMatrix4fv(mObjectColor.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(idenity));
    mCubeColorMesh.Render();
    glUseProgram(0);
    glEnable(GL_DEPTH_TEST);
  }
}

void WorldScene::framebufferSizeChanged(int w, int h)
{
  glutil::MatrixStack persMatrix;
  persMatrix.Perspective(45.0f, (w / static_cast<float>(h)), g_fzNear, g_fzFar);

  glUseProgram(mUniformColor.theProgram);
  glUniformMatrix4fv(mUniformColor.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
  glUseProgram(mObjectColor.theProgram);
  glUniformMatrix4fv(mObjectColor.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
  glUseProgram(mUniformColorTint.theProgram);
  glUniformMatrix4fv(mUniformColorTint.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
  glUseProgram(0);

  glViewport(0, 0, w, h);
}
