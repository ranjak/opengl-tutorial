/**
 * This shader implements a modified version of the Oren-Nayar qualitative
 * reflectance model, proposed by Yasuhiro Fujii.
 * See here: http://mimosa-pudica.net/improved-oren-nayar.html
 */
#version 330

uniform vec4 ambientIntensity;
uniform vec4 lightIntensity;
// Positions in model space
uniform vec3 lightPos;
uniform vec3 cameraPos;
// Roughness parameter, from 0.0 to 1.0
uniform float sigma;

smooth in vec4 interpColor;
// in model space
smooth in vec3 fragmentPos;
smooth in vec3 vertexNormal;

out vec4 outputColor;

const float PI = 3.14159265359;
const float sigmaFactor = PI/2.0f - 2.0f/3.0f;

void main()
{
  vec3 dirToLight = normalize(lightPos - fragmentPos);
  vec3 viewDir = normalize(cameraPos - fragmentPos);
  vec3 normal = normalize(vertexNormal);

  // Z = zenith angle, A = azimuth angle
  // Angle from surface to light
  float cosZi = dot(normal, dirToLight);

  // Light coming from behind -> reflectance = 0
  if (cosZi <= 0.0f) {
    outputColor = interpColor * ambientIntensity;
    return;
  }
  // Angle from surface to camera
  float cosZr = dot(normal, viewDir);

  float termA = 1.0f / (PI + sigmaFactor * sigma);

  float termB = sigma / (PI + sigmaFactor * sigma);
  float cosAzimuthSinaTanb = (dot(dirToLight, viewDir) - cosZr * cosZi) / max(cosZr, cosZi);

  // Terms A and B divide by Pi, cancel that to be able to multiply with colors directly.
  vec4 orenNayarColor = interpColor * PI * cosZi * (termA + termB * max(0.0f, cosAzimuthSinaTanb)) * lightIntensity;

  outputColor = orenNayarColor + (interpColor * ambientIntensity);
}
