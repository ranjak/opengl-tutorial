#version 330

smooth in vec4 vertexColor;

out vec4 outputColor;

void main(void)
{
  outputColor = vertexColor;
}
