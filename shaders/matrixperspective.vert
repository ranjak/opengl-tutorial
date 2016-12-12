#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform vec2 offset;
uniform mat4 projectionMatrix;

smooth out vec4 vertexColor;

void main(void)
{
  vec4 cameraPos = position + vec4(offset.x, offset.y, 0.0, 0.0);

  gl_Position = projectionMatrix * cameraPos;
  vertexColor = color;
}
