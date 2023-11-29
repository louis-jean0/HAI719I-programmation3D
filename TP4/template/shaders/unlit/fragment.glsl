#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in mat3 TBN;
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D colorTexture;
uniform sampler2D bumpTexture;

void main() {

    vec3 bumpNormal = texture(bumpTexture,o_uv0).xyz;
    bumpNormal = bumpNormal * 2.0 - 1.0;

    // On passe de l'espace de la tangente à l'espace du modèle
    vec3 normal = normalize(TBN * bumpNormal);

    //FragColor = color;
    FragColor = texture(colorTexture, o_uv0) * color;
    // DEBUG: position
    //FragColor = vec4(o_positionWorld, 1.0);
    // DEBUG: normal
    //FragColor = vec4(0.5 * o_normalWorld + vec3(0.5) , 1.0);
    // DEBUG: uv0
    // FragColor = vec4(o_uv0, 1.0);
}
