#include "transform.hpp"
#include <glm/mat4x4.hpp>

namespace ogl
{

glm::mat4 makeProjectionMat(float zNear, float zFar, float frustumScale, float aspectRatio)
{
  return glm::mat4 {
    frustumScale / aspectRatio, 0.0, 0.0, 0.0,
    0.0, frustumScale, 0.0, 0.0,
    0.0, 0.0, (-zNear - zFar)/(zNear - zFar), -1.0,
    0.0, 0.0,  2*zNear*zFar/(zNear - zFar), 0.0
  };
}

} // namespace ogl
