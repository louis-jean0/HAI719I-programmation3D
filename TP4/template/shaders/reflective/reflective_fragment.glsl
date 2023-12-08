#version 330 core

in vec3 normalWorld;
in vec3 positionWorld;

out vec4 FragColor;

uniform vec3 camPos;
uniform samplerCube skybox;

void main() {
    vec3 incident = normalize(positionWorld - camPos);
    vec3 reflected = reflect(incident,normalWorld); // normalWorld déjà normalisé dans vertex
    FragColor = vec4(texture(skybox,reflected).rgb,1.0);
    // DEBUG
    //FragColor = vec4(normalWorld * 0.5 + 0.5,1.0);
    //FragColor = vec4(positionWorld,1.0);
}

