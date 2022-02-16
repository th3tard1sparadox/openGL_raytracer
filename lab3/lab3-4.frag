#version 150

in vec3 normal;
in vec3 surfacePos;

out vec4 out_Color;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

void main(void)
{
  const vec3 light = vec3(0.6, 0.6, 0.6);
  float shading = dot(normalize(normal), light);
  shading = clamp(shading, 0, 1);
  out_Color = vec4(shading, shading, shading, 1.0);
}
