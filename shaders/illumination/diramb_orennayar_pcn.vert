#version 330

const float PI = 3.14159265359;

mat3 rotateX(float angleRad)
{
  float cosa = cos(angleRad);
  float sina = sin(angleRad);
  return mat4(
        1.0, 0.0, 0.0,
        0.0, cosa, sina,
        0.0, -sina, cosa);
}

mat3 rotateY(float angleRad)
{
  float cosa = cos(angleRad);
  float sina = sin(angleRad);
  return mat4(
        cosa, 0.0, -sina,
        0.0, 1.0, 0.0,
        sina, 0.0, cosa);
}

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 diffuseColor;
layout(location = 2) in vec3 normal;

smooth out vec4 interpColor;

uniform vec3 dirToLight;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;
// Polar angles of surface facets. x = zenith, y = azimuth
uniform vec2 facetAngles;

uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

layout(std140) uniform Projection
{
  mat4 cameraToClipMatrix;
};

void main()
{
  gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));

  vec3 facetNormModelSpace = rotateX(facetAngles.x) * rotateY(facetAngles.y) * normal;
  vec3 normCamSpace = normalize(normalModelToCameraMatrix * normal);
  vec3 facetNorm = normalize(normalModelToCameraMatrix * facetNormModelSpace);

  // In camera space, the view is always pointing forward (negative Z).
  vec3 viewDir = vec3(0.0f, 0.0f, -1.0f);

  vec4 orenNayarColor = (diffuseColor / PI) * lightIntensity * (dot(dirToLight, facetNorm) * dot(viewDir, facetNorm)) / (dot(facetNorm, normCamSpace) * dot(viewDir, normCamSpace));

  interpColor = orenNayarColor + (diffuseColor * ambientIntensity);
}
