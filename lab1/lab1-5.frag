#version 150

out vec4 out_Color;
in vec3 in_color;

void main(void)
{
	out_Color = vec4(in_color, 1.0);
}
