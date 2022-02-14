#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
uniform mat4 lookMatrix;
uniform mat4 mdlMatrix;
uniform mat4 projectionMatrix;
out vec3 normal;

void main(void)
{
  mat3 normalMatrix = mat3(mdlMatrix);
  normal = in_Normal * normalMatrix;


  gl_Position = projectionMatrix * lookMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
