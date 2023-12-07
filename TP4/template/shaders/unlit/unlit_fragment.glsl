#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in mat3 TBN;
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D colorTexture;
uniform sampler2D normalMapTexture;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 camPos;

void main() {

    vec3 normalMapNormal = texture(normalMapTexture, o_uv0).xyz;
    normalMapNormal = normalMapNormal * 2.0 - 1.0;

    // On passe de l'espace de la tangente à l'espace monde
    vec3 normal = normalize(TBN * normalMapNormal);

    // Phong

    // Ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 lightDir = normalize(lightPosition - o_positionWorld);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - o_positionWorld);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * texture(colorTexture, o_uv0).rgb;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(texture(colorTexture, o_uv0).rgb,1.0);

}
