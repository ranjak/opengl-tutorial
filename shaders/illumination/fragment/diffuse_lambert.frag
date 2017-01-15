#version 330

uniform vec4 ambientIntensity;
uniform vec4 lightIntensity;
uniform vec3 lightPos; // in model space

smooth in vec4 interpColor;
// in model space
smooth in vec3 fragmentPos;
smooth in vec3 vertexNormal;

out vec4 outputColor;

void main()
{
  vec3 dirToLight = normalize(lightPos - fragmentPos);
  float cosLV = dot(dirToLight, normalize(vertexNormal));

  outputColor = interpColor * lightIntensity * max(0.0f, cosLV) +
      ambientIntensity * interpColor;
}
