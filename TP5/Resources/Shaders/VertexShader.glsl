#version 450 core // Minimal GL version support expected from the GPU

layout(location=0) in vec3 vPosition; // The 1st input attribute is the position (CPU side: glVertexAttrib 0)
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoord;

uniform mat4 projectionMat, modelMat, viewMat, normalMat, depthMVP; 

out vec3 fPosition;
out vec4 fPositionWorldSpace;
out vec4 fPositionLightSpace;
out vec3 fNormal;
out vec2 fTexCoord;

void main() {
    
    fPosition = vPosition;
    fPositionWorldSpace = modelMat * vec4(fPosition,1.0);
    fNormal = normalize(normalMat * vec4(vNormal,1.0)).xyz;
    fTexCoord = vTexCoord;
    fPositionLightSpace = depthMVP * fPositionWorldSpace;
    gl_Position = projectionMat * viewMat * modelMat * vec4(vPosition,1.0);

}
