#version 150

in vec3 in_Position;
in vec2 inTexCoord;

uniform mat4 viewToWorldMatrix;
uniform mat4 modelToViewMatrix;
uniform mat4 worldToScreenMatrix;

out vec2 texCoord;

void main(void)
{
  texCoord = inTexCoord;

  gl_Position = worldToScreenMatrix * modelToViewMatrix * vec4(in_Position, 1.0);
}
