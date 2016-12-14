#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/fwd.hpp>
#include <cmath>

namespace ogl
{

constexpr double PId = 3.141592653589793;
constexpr float PIf = 3.1415927f;

template<typename T>
constexpr float degToRadf(T degrees)
{
  return degrees * (PIf / 180.0f);
}

template<typename T, typename U, typename V>
void rotateZ(T& point, const U& center, V degrees)
{
  float rads = degToRadf(degrees);
  float sinRot = std::sin(rads);
  float cosRot = std::cos(rads);

  T vec, vecRot;
  vec.x = point.x - center.x;
  vec.y = point.y - center.y;
  vecRot.x = vec.x * cosRot - vec.y * sinRot;
  vecRot.y = vec.x * sinRot + vec.y * cosRot;

  point.x = center.x + vecRot.x;
  point.y = center.y + vecRot.y;
}

glm::mat4 makeProjectionMat(float zNear, float zFar, float frustumScale, float aspectRatio);

} // namespace ogl

#endif // TRANSFORM_HPP
