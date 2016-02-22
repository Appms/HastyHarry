#version 430 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 LightDir;
in vec2 TexCoords;
in vec3 ViewDir;

struct LightInfo {
    vec4 Position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
};
uniform LightInfo Light;

struct MaterialInfo {
    sampler2D DiffuseTexture;
    sampler2D NormalTexture;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

out vec4 finalColor;

const int levels = 3;
const float scaleFactor = 1.0 / levels;

vec3 phongModel( vec3 norm, vec3 diffR ) {
    vec3 r = reflect( -LightDir, norm );
    vec3 ambient = Light.La * Material.Ka;
    float sDotN = max( dot(LightDir, norm), 0.0 );
    vec3 diffuse = Light.Ld * Material.Kd * diffR * floor(sDotN * levels) * scaleFactor;

    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Ls * Material.Ks *
               pow( max( dot(r,ViewDir), 0.0 ),
                       Material.Shininess );

    return ambient + diffuse + spec;
}

void main( void ) {
    // Lookup the normal from the normal map
    vec4 normal = 2.0 * texture( Material.NormalTexture, TexCoords ) -
                        1.0;

    // The color texture is used as the diff. reflectivity
    vec4 texColor = texture( Material.DiffuseTexture, TexCoords );

    finalColor = vec4( phongModel(normal.xyz, texColor.rgb),
                      1.0 );

    //finalColor = normal;
}

