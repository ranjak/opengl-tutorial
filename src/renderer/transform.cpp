#include "transform.hpp"
#include <glm/mat4x4.hpp>

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

glm::mat3 rotateZ(float degrees)
{
  float rads = degToRadf(degrees);
  float cosa = std::cos(rads);
  float sina = std::sin(rads);

  return glm::mat3 {
    cosa, sina, 0.0f,
    -sina, cosa, 0.0f,
    0.0f, 0.0f, 1.0f
  };
}

glm::mat3 rotateY(float degrees)
{
  float rads = degToRadf(degrees);
  float cosa = std::cos(rads);
  float sina = std::sin(rads);

  return glm::mat3 {
    cosa, 0.0f, -sina,
    0.0f, 1.0f, 0.0f,
    sina, 0.0f, cosa
  };
}

glm::mat3 rotateX(float degrees)
{
  float rads = degToRadf(degrees);
  float cosa = std::cos(rads);
  float sina = std::sin(rads);

  return glm::mat3 {
    1.0f, 0.0f, 0.0f,
    0.0f, cosa, sina,
    0.0f, -sina, cosa
  };
}

} // namespace ogl
