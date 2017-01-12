#version 330

const float PI = 3.14159265359;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 diffuseColor;
layout(location = 2) in vec3 normal;

smooth out vec4 interpColor;

uniform vec3 dirToLight;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;
// Stadard deviation of facet slopes, in radians
uniform float sigma;

uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

layout(std140) uniform Projection
{
  mat4 cameraToClipMatrix;
};

void main()
{
  gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));

  vec3 normCamSpace = normalize(normalModelToCameraMatrix * normal);
  vec3 viewDir = normalize(-vec3(modelToCameraMatrix * vec4(position, 1.0)));

  // Z = zenith angle, A = azimuth angle
  // Angle from surface to light
  float cosZi = max(0.0f, dirToLight.z);
  float sinZi = sqrt(1.0f - cosZi*cosZi);
  float cosAi = dirToLight.x / sinZi;
  float sinAi = dirToLight.y / sinZi;

  // Angle from surface to camera
  float cosZr = viewDir.z;
  float sinZr = sqrt(1.0f - cosZr*cosZr);
  float cosAr = viewDir.x / sinZr;
  float sinAr = viewDir.y / sinZr;

  float sigma2 = sigma*sigma;
  float termA = 1.0f - 0.5f * sigma2 / (sigma2 + 0.57f);

  float termB = 0.45f * sigma2 / (sigma2 + 0.09f);
  termB *= max(0.f, cosAi*cosAr + sinAi*sinAr);
  // sin(alpha) part. Find out max(Zi, Zr). Zi > Zr if cosZi < cosZr.
  termB *= (cosZi < cosZr) ? sinZi : sinZr;
  // tan(beta) part
  termB *= (cosZi > cosZr) ? sinZi/cosZi : sinZr/cosZr;

  vec4 orenNayarColor = (diffuseColor / PI) * cosZi * (termA + termB) * lightIntensity;

  interpColor = orenNayarColor + (diffuseColor * ambientIntensity);
}
