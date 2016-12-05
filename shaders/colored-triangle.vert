#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

smooth out vec4 vertexColor;

void main(void)
{
  gl_Position = position;
  vertexColor = color;
}
