#version 330

smooth in vec4 vertexColor;

uniform float fadeLoopDuration;
uniform float currentTime;

out vec4 outputColor;

const vec4 fadeColor = vec4(0.0, 0.0, 0.0, 1.0);

void main(void)
{
  float loopTime = mod(currentTime, fadeLoopDuration);
  float lerpValue = loopTime / fadeLoopDuration;

  if (lerpValue < 0.5) {
    outputColor = mix(vertexColor, fadeColor, lerpValue * 2.0);
  }
  else {
    outputColor = mix(fadeColor, vertexColor, lerpValue * 2.0 - 1.0);
  }
}
