#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 diffuseColor;
layout(location = 2) in vec3 normal;

smooth out vec4 interpColor;
smooth out vec3 fragmentPos;
smooth out vec3 vertexNormal;

uniform mat4 modelToCameraMatrix;

uniform Projection
{
  mat4 cameraToClipMatrix;
};

void main()
{
  gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));

  interpColor = diffuseColor;
  fragmentPos = position;
  vertexNormal = normal;
}
