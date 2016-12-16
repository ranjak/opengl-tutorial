#ifndef POINTYSHAPE_HPP
#define POINTYSHAPE_HPP

#include <glad/glad.h>

namespace pointyshape
{
namespace
{

const int numVertices = 8;

#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

const float vertexData[] =
{
  +1.0f, +1.0f, +1.0f,
  -1.0f, -1.0f, +1.0f,
  -1.0f, +1.0f, -1.0f,
  +1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  +1.0f, +1.0f, -1.0f,
  +1.0f, -1.0f, +1.0f,
  -1.0f, +1.0f, +1.0f,

  GREEN_COLOR,
  BLUE_COLOR,
  RED_COLOR,
  BROWN_COLOR,

  GREEN_COLOR,
  BLUE_COLOR,
  RED_COLOR,
  BROWN_COLOR,
};

const GLushort indexData[] =
{
  0, 1, 2,
  1, 0, 3,
  2, 3, 0,
  3, 2, 1,

  5, 4, 6,
  4, 5, 7,
  7, 6, 4,
  6, 7, 5,
};

const int numIndices = sizeof(indexData) / sizeof(indexData[0]);

} // namespace
} // namespace pointyshape

#endif // POINTYSHAPE_HPP
