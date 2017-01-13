#version 330

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

  /*** Oren-Nayar reflectance computation (simplified model) ***/

  // Z = zenith angle, A = azimuth angle
  // Angle from surface to light
  float cosZi = dot(normCamSpace, dirToLight);

  // Light coming from behind -> reflectance = 0
  if (cosZi <= 0.0f) {
    interpColor = diffuseColor * ambientIntensity;
    return;
  }
  // Angle from surface to camera
  float cosZr = dot(normCamSpace, viewDir);

  // Using vector math, the formula can be reduced down to
  // using only 3 dot products (no sine, sqrt, etc.)
  float sigma2 = sigma*sigma;
  float termA = 1.0f - 0.5f * sigma2 / (sigma2 + 0.57f);

  float termB = 0.45f * sigma2 / (sigma2 + 0.09f);
  float cosAzimuthSinaTanb = (dot(dirToLight, viewDir) - cosZr * cosZi) / max(cosZr, cosZi);

  vec4 orenNayarColor = diffuseColor * cosZi * (termA + termB * max(0.0f, cosAzimuthSinaTanb)) * lightIntensity;

  interpColor = orenNayarColor + (diffuseColor * ambientIntensity);
}
