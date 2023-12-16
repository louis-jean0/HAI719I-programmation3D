#version 450 core // Minimal GL version support expected from the GPU

struct LightSource {
    vec3 position;
    vec3 color;
    float intensity;
    int isActive;
};

int nbLights = 3;

uniform LightSource lightSources[3];
uniform sampler2D shadowMap[3];

struct Material {
    vec3 albedo;
    float shininess;
};

uniform Material material;

in vec3 fPosition;
in vec4 fPositionWorldSpace; // Shader input, linearly interpolated by default from the previous stage (here the vertex shader)
in vec4 fPositionLightSpace;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 colorResponse; // Shader output: the color response attached to this fragment

uniform mat4 projectionMat, viewMat, modelMat, normalMat;

float pi = 3.1415927;

float shadowCalculation(vec4 positionLightSpace, int lightIndex) {
    vec3 projCoords = positionLightSpace.xyz / positionLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // Transformation de l'espace [-1,1] en [0,1]
    float closestDepth = texture(shadowMap[lightIndex], projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 0.5 : 1.0; // Simple test d'ombre
    return shadow;
}


void main() {
    
    vec3 n = fNormal;

    // Linear barycentric interpolation does not preserve unit vectors
    vec3 wo = normalize(-fPosition); // unit vector pointing to the camera
    vec3 radiance = vec3(0,0,0);

    if( dot( n , wo ) >= 0.0 ) {
        {
            for(int i = 0; i < nbLights; i++) {
                if( lightSources[i].isActive == 1 ) { // WE ONLY CONSIDER LIGHTS THAT ARE SWITCHED ON
                    {
                        vec3 wi = normalize (vec3((viewMat * modelMat * vec4(lightSources[i].position,1)).xyz) - fPosition); // unit vector pointing to the light source (change if you use several light sources!!!)
                        if(dot(wi,n) >= 0.0) { // WE ONLY CONSIDER LIGHTS THAT ARE ON THE RIGHT HEMISPHERE (side of the tangent plane)
                            vec3 wh = normalize( wi + wo ); // half vector (if wi changes, wo should change as well)
                            vec3 Li = lightSources[i].color * lightSources[i].intensity;
                            float shadow = shadowCalculation(fPositionLightSpace,i);
                            radiance = radiance +
                                    Li // light color
                                    * material.albedo
                                    * ( max(dot(n,wi),0.0) + pow(max(dot(n,wh),0.0),material.shininess) )
                                    * shadow
                                    ;
                        }
                    }
                }
            }
        }
    colorResponse = vec4(radiance, 1.0); // Building an RGBA value from an RGB one.
    colorResponse = vec4(fNormal,1.0);
    //colorResponse = fNormal;
    }
}