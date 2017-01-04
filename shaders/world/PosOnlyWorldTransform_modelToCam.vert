#version 330

layout(location = 0) in vec4 position;

layout(std140) uniform GlobalMatrices
{
  mat4 cameraToClipMatrix;
};

uniform mat4 modelToCameraMatrix;

void main()
{
  vec4 temp = modelToCameraMatrix * position;
  gl_Position = cameraToClipMatrix * temp;
}
