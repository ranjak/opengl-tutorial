#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
layout(location=2) in vec3 normal;

uniform mat4 modelToCam;
uniform mat3 normalModelToCam;
uniform mat4 camToClip;

uniform vec3 lightDirection;
uniform vec4 lightIntensity;

smooth out vec4 diffuseColor;

void main(void)
{
  gl_Position = camToClip * modelToCam * position;

  // Compute diffuse lighting (Gouraud)
  vec4 normalCam = normalModelToCam * normal;
  float incidenceAngleCos = dot(normalize(normalCam), normalize(-lightDirection));
  diffuseColor = lightIntensity * color * clamp(incidenceAngleCos, 0.0f, 1.0f);
}
