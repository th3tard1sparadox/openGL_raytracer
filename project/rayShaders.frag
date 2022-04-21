#version 150

in vec2 resolution;
in vec4 gl_FragCoord;
in vec2 texCoord;

out vec4 out_Color;

void main(void)
{
    vec2 position = gl_FragCoord.xy / resolution.xy;
    out_Color = vec4(texCoord.x, texCoord.y, 1.0, 1.0);
}