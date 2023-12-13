#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 uv0;

out vec3 normalWorld;
out vec3 positionWorld;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
    normalWorld = normalize(mat3(transpose(inverse(model))) * normal);
    positionWorld = (model * vec4(position,1.0)).xyz;
    gl_Position = projection * view * model * vec4(position,1.0); 
}