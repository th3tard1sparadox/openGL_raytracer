#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
uniform mat4 xTranslationMatrix;
uniform mat4 xRotationMatrix;
uniform mat4 zRotationMatrix;
out vec3 normal;
out vec2 texCoord;

void main(void)
{
  normal = in_Normal;
  texCoord = inTexCoord;
  gl_Position = xRotationMatrix * zRotationMatrix * vec4(in_Position, 1.0);
}
