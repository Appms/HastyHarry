#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

uniform mat4 modelMatrix;
uniform vec3 cameraPosition;

uniform float materialShininess;
uniform vec3 materialSpecularColor;
uniform sampler2D diffuseTexture;

uniform float ambientCoefficient;

#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light {
    vec4 position;
    vec3 intensities; //color and brightness
    float attenuation;
    vec3 direction;
    float coneAngle;
} lights[MAX_LIGHTS];

out vec4 finalColor;

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {
    vec3 surfaceToLight;
    float attenuation = 1.0;
    if(light.position.w == 0.0) {
        //directional light
        surfaceToLight = normalize(-light.direction.xyz);
        attenuation = 1.0;
    } else {
        //point light
        surfaceToLight = normalize(light.position.xyz - surfacePos);
        float distanceToLight = length(light.position.xyz - surfacePos);
        //simplified version of attenuation (0 = no attenuation)
        attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

        //cone
        //1. dot of -surfaceToLight (LightToSurface) and light direction
        //2. arcus cos of that to deg is the light To surface angle
        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.direction))));
        if(lightToSurfaceAngle > light.coneAngle){
            attenuation = 0.0;
        }
    }

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;

    //specular
    //TODO How do I get a fade at the borders??
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        //1. reflect LightToSurface around normal
        //2. that to the power of the shnininess
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

    //color before gamma correction
    return attenuation*(diffuse + specular);
}

void main( void ) {
    //TODO Find out why I need to do this
    vec3 normal = normalize(transpose(inverse(mat3(modelMatrix))) * fragNormal);

    vec3 surfacePos = vec3(modelMatrix * vec4(fragVert, 1));
    vec4 surfaceColor = texture(diffuseTexture, fragTexCoord);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

    //combine color from all the lights
    vec3 linearColor = vec3(0);
    for(int i = 0; i < numLights; ++i){
        linearColor += ApplyLight(lights[i], surfaceColor.rgb, normal, surfacePos, surfaceToCamera);
    }

    //ambient
    vec3 ambient = ambientCoefficient * surfaceColor.rgb;

    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor + ambient, gamma), surfaceColor.a);
}

