#version 150

out vec4 out_Color;
in float shading;

uniform vec3 inColor;
void main(void)
{
  out_Color = vec4(inColor*shading, 1.0);
}