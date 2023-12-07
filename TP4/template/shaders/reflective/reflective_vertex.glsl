#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 uv0;

out vec3 normalWorld;
out vec3 positionWorld;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
    normalWorld = normalize(mat3(transpose(inverse(modelMatrix))) * normal);
    positionWorld = (modelMatrix * vec4(position,1.0)).xyz;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position,1.0); 
}