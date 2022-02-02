#version 150

out vec4 out_Color;
in float shading;
in vec2 texCoord;
uniform sampler2D texUnit;

void main(void)
{
  out_Color = texture(texUnit, texCoord)*vec4(shading, shading, shading, 1.0);
}
