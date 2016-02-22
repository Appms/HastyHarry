//DIFFUSE COLOR VERTEX SHADER

#version 430 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 TextureCoords;
in vec4 VertexTangent;

struct LightInfo {
    vec4 Position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
};
uniform LightInfo Light;

uniform   mat4 ProjectionMatrix;
uniform   mat4 ModelViewMatrix;
uniform   mat3 NormalMatrix;

out vec3 LightDir;
out vec2 TexCoords;
out vec3 ViewDir;

void main()  {

    //Transform normal and tangent to eye space
    vec3 norm = normalize(NormalMatrix * VertexNormal);
    vec3 tang = normalize(NormalMatrix * VertexTangent.xyz);

    //Compute the bitangent
    vec3 bitangent = normalize(cross(norm, tang))*VertexTangent.w;

    //Matrix for transformation to tangent space
    mat3 toTangSpace = mat3(
    tang.x, bitangent.x, norm.x,
    tang.y, bitangent.y, norm.y,
    tang.z, bitangent.z, norm.z);

    //Get the position in eye coordinates
    vec3 pos = vec3(ModelViewMatrix * vec4(VertexPosition,1.0));

	//vec3 light = vec3(ModelViewMatrix * Light.Position);

    //Transform light dir and view dir to tangent space
    LightDir = normalize(toTangSpace * (Light.Position.xyz - pos));
    ViewDir = toTangSpace * normalize(-pos);

    //Pass the texture coordinates
    TexCoords = TextureCoords;

    gl_Position = ProjectionMatrix * vec4(pos,1.0);

}
