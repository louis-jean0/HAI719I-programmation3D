#version 330 core

in vec2 o_uv0;
in vec3 o_positionWorld;
in vec3 o_normalWorld;
in mat3 TBN;

uniform sampler2D colorTexture;
uniform sampler2D normalMapTexture;
uniform sampler2D pbr_packed_texture;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 camPos;

out vec4 FragColor;

const float PI = 3.14159265359;
vec3 Lo = vec3(0.0);

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {

    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;

}

float GeometrySchlickGGX(float NdotV, float roughness) {

    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;

}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {

    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;

}

void main() {

    vec3 normalMapNormal = texture(normalMapTexture, o_uv0).xyz;
    normalMapNormal = normalMapNormal * 2.0 - 1.0;

    // On passe de l'espace de la tangente à l'espace monde
    vec3 normal = normalize(TBN * normalMapNormal);

    vec3 albedo = texture(colorTexture,o_uv0).rgb;
    float ao = texture(pbr_packed_texture,o_uv0).r;
    float roughness = texture(pbr_packed_texture,o_uv0).g;
    float metalness = texture(pbr_packed_texture,o_uv0).b;

    vec3 viewDir = normalize(camPos - o_positionWorld);
    vec3 lightDir = normalize(lightPosition - o_positionWorld);
    vec3 halfwayVector = normalize(viewDir + lightDir);
    float distance = length(lightPosition - o_positionWorld);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColor * attenuation;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0,albedo,metalness);
    vec3 F = fresnelSchlick(max(dot(halfwayVector,viewDir),0.0),F0);

    float NDF = DistributionGGX(normal, halfwayVector, roughness);       
    float G = GeometrySmith(normal, viewDir, lightDir, roughness);

    // Cook-Torrance
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0)  + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalness;

    float NdotL = max(dot(normal, lightDir), 0.0);        
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color,1.0);

}