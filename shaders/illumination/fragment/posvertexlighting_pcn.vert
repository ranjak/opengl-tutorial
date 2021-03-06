#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 diffuseColor;
layout(location = 2) in vec3 normal;

smooth out vec4 interpColor;

uniform vec3 lightPos; // In model space
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform mat4 modelToCameraMatrix;

uniform Projection
{
  mat4 cameraToClipMatrix;
};

void main()
{
  gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));

  vec3 dirToLight = normalize(lightPos - position);

  float cosAngIncidence = dot(normal, dirToLight);
  cosAngIncidence = clamp(cosAngIncidence, 0, 1);

  interpColor = (diffuseColor * lightIntensity * cosAngIncidence) +
    (diffuseColor * ambientIntensity);
}
