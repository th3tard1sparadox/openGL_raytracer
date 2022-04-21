#version 150

in  vec3 in_Position;
in  vec2 in_texCoord;

out vec2 texCoord;
void main(void)
{
    gl_Position = vec4(in_Position, 1.0);
    texCoord = in_texCoord;
}
