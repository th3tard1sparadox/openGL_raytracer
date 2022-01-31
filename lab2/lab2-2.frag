#version 150

out vec4 out_Color;
in vec3 normal;
in vec2 texCoord;
uniform float t;

void main(void)
{
        float a = sin(texCoord.s * t * 30);
        float b = texCoord.t;
        out_Color = vec4(a, b, 1.0, 0.5);
}
