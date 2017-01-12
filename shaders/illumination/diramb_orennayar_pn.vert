#version 330

layout(location = 0) in vec3 position;
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
    interpColor =  ambientIntensity;
    return;
  }

  float sinZi = sqrt(1.0f - cosZi*cosZi);

  // Angle from surface to camera
  float cosZr = dot(normCamSpace, viewDir);
  float sinZr = sqrt(1.0f - cosZr*cosZr);

  // To get the azimuth difference between i and r, project them onto the plane perpendicular to the normal
  vec3 iProj = normalize(cross(normCamSpace, cross(dirToLight, normCamSpace)));
  vec3 rProj = normalize(cross(normCamSpace, cross(viewDir, normCamSpace)));
  float cosAzimuthDiff = dot(iProj, rProj);

  float sigma2 = sigma*sigma;
  float termA = 1.0f - 0.5f * sigma2 / (sigma2 + 0.57f);

  float termB = 0.45f * sigma2 / (sigma2 + 0.09f);
  termB *= max(0.f, cosAzimuthDiff);
  // sin(alpha) part. Find out max(Zi, Zr). Zi > Zr if cosZi < cosZr.
  termB *= (cosZi < cosZr) ? sinZi : sinZr;
  // tan(beta) part
  termB *= (cosZi > cosZr) ? sinZi/cosZi : sinZr/cosZr;

  vec4 orenNayarColor =  cosZi * (termA + termB) * lightIntensity;

  interpColor = orenNayarColor + ambientIntensity;
}
