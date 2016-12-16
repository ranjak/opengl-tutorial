#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 modelToViewMatrix;
uniform mat4 projectionMatrix;

smooth out vec4 vertexColor;

void main(void)
{
  gl_Position = projectionMatrix * modelToViewMatrix * position;
  vertexColor = color;
}
