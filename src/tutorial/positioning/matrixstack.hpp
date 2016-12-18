#ifndef MATRIXSTACK_HPP
#define MATRIXSTACK_HPP

#include "transform.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <stack>

class MatrixStack
{
public:
  MatrixStack() : mCurrentMat(1.0f), mMatrices() {}

  void push()
  {
    mMatrices.push(mCurrentMat);
  }

  void pop()
  {
    mCurrentMat = mMatrices.top();
    mMatrices.pop();
  }

  const glm::mat4& top()
  {
    return mCurrentMat;
  }

  void rotateX(float degrees)
  {
    mCurrentMat = mCurrentMat * glm::mat4(ogl::rotateX(degrees));
  }

  void rotateY(float degrees)
  {
    mCurrentMat = mCurrentMat * glm::mat4(ogl::rotateY(degrees));
  }

  void rotateZ(float degrees)
  {
    mCurrentMat = mCurrentMat * glm::mat4(ogl::rotateZ(degrees));
  }

  void scale(float x, float y, float z)
  {
    glm::mat4 scaleMat(1.0f);
    scaleMat[0].x = x;
    scaleMat[1].y = y;
    scaleMat[2].z = z;

    mCurrentMat = mCurrentMat * scaleMat;
  }

  void scale(const glm::vec3& factors)
  {
    scale(factors.x, factors.y, factors.z);
  }

  void translate(float x, float y, float z)
  {
    glm::mat4 transMat(1.0f);
    transMat[3].x = x;
    transMat[3].y = y;
    transMat[3].z = z;

    mCurrentMat = mCurrentMat * transMat;
  }

  void translate(const glm::vec3& offset)
  {
    glm::mat4 transMat(1.0f);
    transMat[3] = glm::vec4(offset, 1.0);

    mCurrentMat = mCurrentMat * transMat;
  }

private:
  glm::mat4 mCurrentMat;
  std::stack<glm::mat4> mMatrices;
};

#endif // MATRIXSTACK_HPP
