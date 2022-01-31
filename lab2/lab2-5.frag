#version 150

out vec4 out_Color;
in vec3 normal;
in vec2 texCoord;
uniform float t;
uniform sampler2D texUnit;

void main(void)
{
        float a = sin(texCoord.s * t * 30);
        float b = texCoord.t;
        out_Color = texture(texUnit, texCoord*5)*vec4(normal, 1.0);
}
