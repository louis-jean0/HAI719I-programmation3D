#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 o_positionWorld;
out vec3 o_normalWorld;
out vec2 o_uv0;
out mat3 TBN;
out vec4 color;

void main() {

  mat3 normalMatrix = mat3(transpose(inverse(model)));
  o_uv0 = uv0;
  o_uv0.y = 1.0 - uv0.y; // Petit trick car mon modèle était lu dans le mauvais sens
  vec4 positionWorld = model * vec4(position, 1.0);
  vec3 T = normalize(normalMatrix * tangent);
  vec3 B = normalize(normalMatrix * bitangent);
  vec3 N = normalize(normalMatrix * normal);
  TBN = transpose(mat3(T, B, N));
  o_positionWorld = positionWorld.xyz;
  o_normalWorld = normalMatrix * normal;
  gl_Position = projection * view * positionWorld;

}
