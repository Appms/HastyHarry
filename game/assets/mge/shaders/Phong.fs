#version 430 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 N;
in vec3 eyeCoords;
in vec2 texCoords0;

struct LightInfo {
    vec4 Position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
};
uniform LightInfo Light[10];

struct MaterialInfo {
    sampler2D DiffuseTexture;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

out vec4 finalColor;

vec3 phong (vec3 N, vec3 L, vec3 V) {

    vec3  ambient  = Material.Ka * Light[0].La;
    vec3  diffuse  = vec3(0.0);
    vec3  specular = vec3(0.0);

    float NdotL    = dot (N,L);

    if (NdotL > 0.0) {
      vec3  R       = reflect(-L, N);;
      float RdotV_n = pow(max(0.0, dot(R,V)), Material.Shininess);

      diffuse  = NdotL   * (Light[0].Ld * Material.Kd);
      specular = RdotV_n * (Light[0].Ls * Material.Ks);
    }

    return (ambient + texture(Material.DiffuseTexture, texCoords0).xyz * diffuse + specular);
}

void main( void ) {
    vec3 n = normalize(N);
    vec3 L = normalize(Light[0].Position.xyz - eyeCoords);
    vec3 V = normalize(-eyeCoords);

    finalColor = vec4(phong(n,L,V),1.0);
}
