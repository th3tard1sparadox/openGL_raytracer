#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

out vec3 normal;
out vec3 surface;

uniform mat4 modelToViewMatrix;
uniform mat4 viewToWorldMatrix;
uniform mat4 worldToScreenMatrix;

void main(void)
{
  normal = normalize(mat3(viewToWorldMatrix) * in_Normal);
  surface = (modelToViewMatrix * vec4(in_Position, 1.0)).xyz;
  gl_Position = worldToScreenMatrix * viewToWorldMatrix * modelToViewMatrix * vec4(in_Position, 1.0);
}
