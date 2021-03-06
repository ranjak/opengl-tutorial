#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <glm/vec4.hpp>

namespace ogl
{
namespace
{

const glm::vec4 triangle[] = {
  {0.75f,  0.75f,  0.0f, 1.0f},
  {0.75f,  -0.75f, 0.0f, 1.0f},
  {-0.75f, -0.75f, 0.0f, 1.0f}
};

glm::vec4 coloredTriangle[] = {
  { 0.0f,    0.5f, 0.0f, 1.0f},
  { 0.5f, -0.366f, 0.0f, 1.0f},
  {-0.5f, -0.366f, 0.0f, 1.0f},
  { 1.0f,    0.0f, 0.0f, 1.0f},
  { 0.0f,    1.0f, 0.0f, 1.0f},
  { 0.0f,    0.0f, 1.0f, 1.0f}
};

glm::vec4 coloredPrism[] = {
  { 0.25f,  0.25f, 0.75f, 1.0f },
  { 0.25f, -0.25f, 0.75f, 1.0f },
  { -0.25f,  0.25f, 0.75f, 1.0f },

  { 0.25f, -0.25f, 0.75f, 1.0f },
  { -0.25f, -0.25f, 0.75f, 1.0f },
  { -0.25f,  0.25f, 0.75f, 1.0f },

  { 0.25f,  0.25f, -0.75f, 1.0f },
  { -0.25f,  0.25f, -0.75f, 1.0f },
  { 0.25f, -0.25f, -0.75f, 1.0f },

  { 0.25f, -0.25f, -0.75f, 1.0f },
  { -0.25f,  0.25f, -0.75f, 1.0f },
  { -0.25f, -0.25f, -0.75f, 1.0f },

  { -0.25f,  0.25f,  0.75f, 1.0f },
  { -0.25f, -0.25f,  0.75f, 1.0f },
  { -0.25f, -0.25f, -0.75f, 1.0f },

  { -0.25f,  0.25f,  0.75f, 1.0f },
  { -0.25f, -0.25f, -0.75f, 1.0f },
  { -0.25f,  0.25f, -0.75f, 1.0f },

  { 0.25f,  0.25f,  0.75f, 1.0f },
  { 0.25f, -0.25f, -0.75f, 1.0f },
  { 0.25f, -0.25f,  0.75f, 1.0f },

  { 0.25f,  0.25f,  0.75f, 1.0f },
  { 0.25f,  0.25f, -0.75f, 1.0f },
  { 0.25f, -0.25f, -0.75f, 1.0f },

  { 0.25f,  0.25f, -0.75f, 1.0f },
  { 0.25f,  0.25f,  0.75f, 1.0f },
  { -0.25f,  0.25f,  0.75f, 1.0f },

  { 0.25f,  0.25f, -0.75f, 1.0f },
  { -0.25f,  0.25f,  0.75f, 1.0f },
  { -0.25f,  0.25f, -0.75f, 1.0f },

  { 0.25f, -0.25f, -0.75f, 1.0f },
  { -0.25f, -0.25f,  0.75f, 1.0f },
  { 0.25f, -0.25f,  0.75f, 1.0f },

  { 0.25f, -0.25f, -0.75f, 1.0f },
  { -0.25f, -0.25f, -0.75f, 1.0f },
  { -0.25f, -0.25f,  0.75f, 1.0f },




  { 0.0f, 0.0f, 1.0f, 1.0f },
  { 0.0f, 0.0f, 1.0f, 1.0f },
  { 0.0f, 0.0f, 1.0f, 1.0f },

  { 0.0f, 0.0f, 1.0f, 1.0f },
  { 0.0f, 0.0f, 1.0f, 1.0f },
  { 0.0f, 0.0f, 1.0f, 1.0f },

  { 0.8f, 0.8f, 0.8f, 1.0f },
  { 0.8f, 0.8f, 0.8f, 1.0f },
  { 0.8f, 0.8f, 0.8f, 1.0f },

  { 0.8f, 0.8f, 0.8f, 1.0f },
  { 0.8f, 0.8f, 0.8f, 1.0f },
  { 0.8f, 0.8f, 0.8f, 1.0f },

  { 0.0f, 1.0f, 0.0f, 1.0f },
  { 0.0f, 1.0f, 0.0f, 1.0f },
  { 0.0f, 1.0f, 0.0f, 1.0f },

  { 0.0f, 1.0f, 0.0f, 1.0f },
  { 0.0f, 1.0f, 0.0f, 1.0f },
  { 0.0f, 1.0f, 0.0f, 1.0f },

  { 0.5f, 0.5f, 0.0f, 1.0f },
  { 0.5f, 0.5f, 0.0f, 1.0f },
  { 0.5f, 0.5f, 0.0f, 1.0f },

  { 0.5f, 0.5f, 0.0f, 1.0f },
  { 0.5f, 0.5f, 0.0f, 1.0f },
  { 0.5f, 0.5f, 0.0f, 1.0f },

  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },

  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },

  { 0.0f, 1.0f, 1.0f, 1.0f },
  { 0.0f, 1.0f, 1.0f, 1.0f },
  { 0.0f, 1.0f, 1.0f, 1.0f },

  { 0.0f, 1.0f, 1.0f, 1.0f },
  { 0.0f, 1.0f, 1.0f, 1.0f },
  { 0.0f, 1.0f, 1.0f, 1.0f }
};

const glm::vec4 perspectivePrism[] = {
   { 0.25f,  0.25f, -1.25f, 1.0f },
   { 0.25f, -0.25f, -1.25f, 1.0f },
  { -0.25f,  0.25f, -1.25f, 1.0f },

   { 0.25f, -0.25f, -1.25f, 1.0f },
  { -0.25f, -0.25f, -1.25f, 1.0f },
  { -0.25f,  0.25f, -1.25f, 1.0f },

   { 0.25f,  0.25f, -2.75f, 1.0f },
  { -0.25f,  0.25f, -2.75f, 1.0f },
   { 0.25f, -0.25f, -2.75f, 1.0f },

   { 0.25f, -0.25f, -2.75f, 1.0f },
  { -0.25f,  0.25f, -2.75f, 1.0f },
  { -0.25f, -0.25f, -2.75f, 1.0f },

  { -0.25f,  0.25f, -1.25f, 1.0f },
  { -0.25f, -0.25f, -1.25f, 1.0f },
  { -0.25f, -0.25f, -2.75f, 1.0f },

  { -0.25f,  0.25f, -1.25f, 1.0f },
  { -0.25f, -0.25f, -2.75f, 1.0f },
  { -0.25f,  0.25f, -2.75f, 1.0f },

   { 0.25f,  0.25f, -1.25f, 1.0f },
   { 0.25f, -0.25f, -2.75f, 1.0f },
   { 0.25f, -0.25f, -1.25f, 1.0f },

   { 0.25f,  0.25f, -1.25f, 1.0f },
   { 0.25f,  0.25f, -2.75f, 1.0f },
   { 0.25f, -0.25f, -2.75f, 1.0f },

   { 0.25f,  0.25f, -2.75f, 1.0f },
   { 0.25f,  0.25f, -1.25f, 1.0f },
  { -0.25f,  0.25f, -1.25f, 1.0f },

   { 0.25f,  0.25f, -2.75f, 1.0f },
  { -0.25f,  0.25f, -1.25f, 1.0f },
  { -0.25f,  0.25f, -2.75f, 1.0f },

   { 0.25f, -0.25f, -2.75f, 1.0f },
  { -0.25f, -0.25f, -1.25f, 1.0f },
   { 0.25f, -0.25f, -1.25f, 1.0f },

   { 0.25f, -0.25f, -2.75f, 1.0f },
  { -0.25f, -0.25f, -2.75f, 1.0f },
  { -0.25f, -0.25f, -1.25f, 1.0f },




  { 0.0f, 0.0f, 1.0f, 1.0f },
  { 0.0f, 0.0f, 1.0f, 1.0f },
  { 0.0f, 0.0f, 1.0f, 1.0f },

  { 0.0f, 0.0f, 1.0f, 1.0f },
  { 0.0f, 0.0f, 1.0f, 1.0f },
  { 0.0f, 0.0f, 1.0f, 1.0f },

  { 0.8f, 0.8f, 0.8f, 1.0f },
  { 0.8f, 0.8f, 0.8f, 1.0f },
  { 0.8f, 0.8f, 0.8f, 1.0f },

  { 0.8f, 0.8f, 0.8f, 1.0f },
  { 0.8f, 0.8f, 0.8f, 1.0f },
  { 0.8f, 0.8f, 0.8f, 1.0f },

  { 0.0f, 1.0f, 0.0f, 1.0f },
  { 0.0f, 1.0f, 0.0f, 1.0f },
  { 0.0f, 1.0f, 0.0f, 1.0f },

  { 0.0f, 1.0f, 0.0f, 1.0f },
  { 0.0f, 1.0f, 0.0f, 1.0f },
  { 0.0f, 1.0f, 0.0f, 1.0f },

  { 0.5f, 0.5f, 0.0f, 1.0f },
  { 0.5f, 0.5f, 0.0f, 1.0f },
  { 0.5f, 0.5f, 0.0f, 1.0f },

  { 0.5f, 0.5f, 0.0f, 1.0f },
  { 0.5f, 0.5f, 0.0f, 1.0f },
  { 0.5f, 0.5f, 0.0f, 1.0f },

  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },

  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },

  { 0.0f, 1.0f, 1.0f, 1.0f },
  { 0.0f, 1.0f, 1.0f, 1.0f },
  { 0.0f, 1.0f, 1.0f, 1.0f },

  { 0.0f, 1.0f, 1.0f, 1.0f },
  { 0.0f, 1.0f, 1.0f, 1.0f },
  { 0.0f, 1.0f, 1.0f, 1.0f },

};

} // namespace
} // namespace ogl

#endif // PRIMITIVES_HPP
