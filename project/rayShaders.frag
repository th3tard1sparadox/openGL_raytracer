#version 150

in vec4 gl_FragCoord;

out vec4 out_Color;

uniform vec4 sphere1;
uniform vec3 light1;

void main(void)
{
    out_Color = vec4(1.0, 0.0, 0.0, 1.0);
}
