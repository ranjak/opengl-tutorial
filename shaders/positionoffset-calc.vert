#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform float pi;
uniform float loopDuration;
uniform float currentTime;

smooth out vec4 vertexColor;

void main(void)
{
  // How far are we in the loop?
  float loopTime = mod(currentTime, loopDuration);

  // Compute the angle corresponding to the time
  float angle = (2.0 * pi * loopTime) / loopDuration;

  float offsetX = 0.5 * cos(angle);
  float offsetY = 0.5 * sin(angle);

  vec4 totalOffset = vec4(offsetX, offsetY, 0.0, 0.0);

  gl_Position = position + totalOffset;
  vertexColor = color;
}
