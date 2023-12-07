#version 330 core
layout (location = 0) in vec3 position;

out vec3 uv;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    uv = position;
    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
}