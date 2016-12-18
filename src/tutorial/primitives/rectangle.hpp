#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <glad/glad.h>

namespace rectangle
{

const int numVertices = 24;

#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f

#define YELLOW_COLOR 1.0f, 1.0f, 0.0f, 1.0f
#define CYAN_COLOR 0.0f, 1.0f, 1.0f, 1.0f
#define MAGENTA_COLOR 	1.0f, 0.0f, 1.0f, 1.0f

const float vertexData[] =
{
  //Front
  +1.0f, +1.0f, +1.0f,
  +1.0f, -1.0f, +1.0f,
  -1.0f, -1.0f, +1.0f,
  -1.0f, +1.0f, +1.0f,

  //Top
  +1.0f, +1.0f, +1.0f,
  -1.0f, +1.0f, +1.0f,
  -1.0f, +1.0f, -1.0f,
  +1.0f, +1.0f, -1.0f,

  //Left
  +1.0f, +1.0f, +1.0f,
  +1.0f, +1.0f, -1.0f,
  +1.0f, -1.0f, -1.0f,
  +1.0f, -1.0f, +1.0f,

  //Back
  +1.0f, +1.0f, -1.0f,
  -1.0f, +1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
  +1.0f, -1.0f, -1.0f,

  //Bottom
  +1.0f, -1.0f, +1.0f,
  +1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, +1.0f,

  //Right
  -1.0f, +1.0f, +1.0f,
  -1.0f, -1.0f, +1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f, +1.0f, -1.0f,


  GREEN_COLOR,
  GREEN_COLOR,
  GREEN_COLOR,
  GREEN_COLOR,

  BLUE_COLOR,
  BLUE_COLOR,
  BLUE_COLOR,
  BLUE_COLOR,

  RED_COLOR,
  RED_COLOR,
  RED_COLOR,
  RED_COLOR,

  YELLOW_COLOR,
  YELLOW_COLOR,
  YELLOW_COLOR,
  YELLOW_COLOR,

  CYAN_COLOR,
  CYAN_COLOR,
  CYAN_COLOR,
  CYAN_COLOR,

  MAGENTA_COLOR,
  MAGENTA_COLOR,
  MAGENTA_COLOR,
  MAGENTA_COLOR,
};

const GLushort indexData[] =
{
  0, 1, 2,
  2, 3, 0,

  4, 5, 6,
  6, 7, 4,

  8, 9, 10,
  10, 11, 8,

  12, 13, 14,
  14, 15, 12,

  16, 17, 18,
  18, 19, 16,

  20, 21, 22,
  22, 23, 20,
};

const int numIndices = sizeof(indexData) / sizeof(indexData[0]);

} // namespace

#endif // RECTANGLE_HPP
