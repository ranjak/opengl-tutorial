#version 330

uniform vec4 ambientIntensity;
uniform vec4 lightIntensity;
// Positions in model space
uniform vec3 lightPos;
uniform vec3 cameraPos;
// Stadard deviation of facet slopes, in radians
uniform float sigma;

smooth in vec4 interpColor;
// in model space
smooth in vec3 fragmentPos;
smooth in vec3 vertexNormal;

out vec4 outputColor;

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

  // Using vector math, the formula can be reduced down to
  // using only 3 dot products (no sine, sqrt, etc.)
  float sigma2 = sigma*sigma;
  float termA = 1.0f - 0.5f * sigma2 / (sigma2 + 0.57f);

  float termB = 0.45f * sigma2 / (sigma2 + 0.09f);
  float cosAzimuthSinaTanb = (dot(dirToLight, viewDir) - cosZr * cosZi) / max(cosZr, cosZi);

  vec4 orenNayarColor = interpColor * cosZi * (termA + termB * max(0.0f, cosAzimuthSinaTanb)) * lightIntensity;

  outputColor = orenNayarColor + (interpColor * ambientIntensity);
}
