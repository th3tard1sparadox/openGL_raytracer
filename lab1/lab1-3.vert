#version 150

in  vec3 in_Position;
uniform mat4 xTranslationMatrix;
uniform mat4 xRotationMatrix;

void main(void)
{
	gl_Position = xRotationMatrix * vec4(in_Position, 1.0);
}
