#version 150

out vec4 out_Color;
in vec3 normal;

void main(void)
{
  const vec3 light = vec3(0.6, 0.6, 0.6);
  float shading = dot(normalize(normal), light);
  shading = clamp(shading, 0, 1);
  out_Color = vec4(shading, shading, shading, 1.0);
}
