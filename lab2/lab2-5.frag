#version 150

out vec4 out_Color;
in vec3 normal;
in vec2 texCoord;
uniform float t;
uniform sampler2D texUnit;

void main(void)
{
        const vec3 light = vec3(0.99, 0.99, 0.99);
        float shading = dot(normalize(normal), light);
        shading = clamp(shading, 0, 1);
        out_Color = texture(texUnit, texCoord*5)*vec4(shading, shading, shading, 1.0);
}
