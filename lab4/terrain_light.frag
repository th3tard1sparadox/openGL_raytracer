#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 normal;
uniform sampler2D tex;

void main(void)
{
    const vec3 light = vec3(0.7, 0.7, 0.7);
    float shading = dot(normalize(normal), light);
    shading = clamp(shading, 0, 1);
    outColor = texture(tex, texCoord)*vec4(shading, shading, shading, 1.0);
}
