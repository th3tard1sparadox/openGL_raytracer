#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
uniform mat4 lookMatrix;
uniform mat4 mdlMatrix;
uniform mat4 projectionMatrix;
out float shading;
out vec2 texCoord;

void main(void)
{
  const vec3 light = vec3(0.99, 0.99, 0.99);
  mat3 normalMatrix = mat3(mdlMatrix);
  vec3 normal = in_Normal * normalMatrix;

  shading = dot(normalize(normal), light);

  texCoord = inTexCoord;
  gl_Position = projectionMatrix * lookMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
