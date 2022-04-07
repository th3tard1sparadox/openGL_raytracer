#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 normal;
in vec3 height;
uniform sampler2D tex1, tex2;

void main(void)
{
    const vec3 light = vec3(0.7, 0.7, 0.7);
    float shading = dot(normalize(normal), light);
    shading = clamp(shading, 0, 1);
    if(height.y > 10) {
      outColor = texture(tex2, texCoord)*vec4(shading, shading, shading, 1.0);
    } else {
      outColor = texture(tex1, texCoord)*vec4(shading, shading, shading, 1.0);
    }
}
