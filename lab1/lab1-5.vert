#version 150

in  vec3 in_Position;
in vec3 Color;
uniform mat4 xTranslationMatrix;
uniform mat4 xRotationMatrix;
uniform mat4 zRotationMatrix;
out vec3 in_color;

void main(void)
{
  in_color = Color;
	gl_Position = xRotationMatrix * zRotationMatrix * vec4(in_Position, 1.0);
}
