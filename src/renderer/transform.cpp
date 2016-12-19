#include "transform.hpp"
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

namespace ogl
{

glm::mat4 makeProjectionMat(float zNear, float zFar, float frustumScale, float aspectRatio)
{
  return glm::mat4 {
    frustumScale / aspectRatio, 0.0, 0.0, 0.0,
    0.0, frustumScale, 0.0, 0.0,
    0.0, 0.0, (-zFar - zNear)/(zFar - zNear), -1.0,
    0.0, 0.0, 2*zNear*zFar/(zFar - zNear), 0.0
  };
}

glm::mat4 makeProjectionMatFov(float zNear, float zFar, float fovAngleV, float aspectRatio)
{
  return makeProjectionMat(zNear, zFar, 1.0f / std::tan(degToRadf(fovAngleV) / 2.0f), aspectRatio);
}

glm::mat4 rotateZ(float degrees)
{
  float rads = degToRadf(degrees);
  float cosa = std::cos(rads);
  float sina = std::sin(rads);

  return glm::mat4 {
    cosa, sina, 0.0f, 0.0f,
    -sina, cosa, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
}

glm::mat4 rotateY(float degrees)
{
  float rads = degToRadf(degrees);
  float cosa = std::cos(rads);
  float sina = std::sin(rads);

  return glm::mat4 {
    cosa, 0.0f, -sina, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    sina, 0.0f, cosa, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
}

glm::mat4 rotateX(float degrees)
{
  float rads = degToRadf(degrees);
  float cosa = std::cos(rads);
  float sina = std::sin(rads);

  return glm::mat4 {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, cosa, sina, 0.0f,
    0.0f, -sina, cosa, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
}

glm::mat4 translate(const glm::vec3& offset)
{
  glm::mat4 mat(1.0f);
  mat[3] = glm::vec4(offset, 1.0);

  return mat;
}

glm::mat4 scale(const glm::vec3& factors)
{
  return glm::mat4 {
    factors.x, 0.0f, 0.0f, 0.0f,
    0.0f, factors.y, 0.0f, 0.0f,
    0.0f, 0.0f, factors.z, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
}

} // namespace ogl
