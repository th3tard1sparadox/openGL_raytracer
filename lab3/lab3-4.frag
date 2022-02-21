#version 150

in vec3 normal;
in vec3 surface;

out vec4 out_Color;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

uniform mat4 viewToWorldMatrix;
uniform vec3 cameraPos;

void main(void)
{
  const vec3 light = vec3(1, 1, 1);
  float shading = dot(normalize(normal), light);
  shading = clamp(shading, 0, 1);

  vec3 totalColor = vec3(0, 0, 0);

  for(int i = 0; i < 4; i++){

    vec3 source = vec3(0,0,0);

    if(isDirectional[i]){
      source = normalize(mat3(viewToWorldMatrix) * lightSourcesDirPosArr[i]);
    } else {
      source = normalize(mat3(viewToWorldMatrix) * (lightSourcesDirPosArr[i] - surface));
    }

    float shading = dot(normalize(normal), source);
    shading = max(0, shading);

    vec3 lightColor = lightSourcesColorArr[i];

    vec3 reflection = reflect(-source, normalize(normal));
    vec3 viewDir = normalize(mat3(viewToWorldMatrix) * (cameraPos - surface));

    float specStrength = 0.0;
    if (dot(source, normalize(normal)) > 0.0) {
      specStrength = dot(reflection, viewDir);
      specStrength = max(specStrength, 0.01);
      specStrength = pow(specStrength, specularExponent);
    }
    totalColor += lightColor * (shading + specStrength);
 }

  out_Color = vec4(totalColor, 1.0);

}
