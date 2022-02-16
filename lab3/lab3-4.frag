#version 150

in vec3 normal;
in vec3 surface;

out vec4 out_Color;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

void main(void)
{
  const vec3 light = vec3(0.6, 0.6, 0.6);
  float shading1 = dot(normal, lightSourcesDirPosArr[2]);
  float shading2 = dot(normal, lightSourcesDirPosArr[3]);
  shading1 = clamp(shading1, 0, shading1);
  shading2 = clamp(shading2, 0, shading2);

  vec3 lightColor1 = lightSourcesColorArr[2];
  vec3 lightColor2 = lightSourcesColorArr[3];
  vec3 totalColor = shading1 * lightColor1  + shading2 * lightColor2;

  out_Color = vec4(totalColor, 1.0);
}
