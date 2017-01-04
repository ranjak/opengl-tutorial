#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

smooth out vec4 interpColor;

uniform mat4 modelToClipMatrix;

void main()
{
  gl_Position = modelToClipMatrix * position;
	interpColor = color;
}
