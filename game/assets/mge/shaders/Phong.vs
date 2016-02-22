//DIFFUSE COLOR VERTEX SHADER

#version 430 // for glsl version (12 is for older versions , say opengl 2.1

uniform   mat4 projectionMatrix;
uniform   mat4 modelViewMatrix;
uniform   mat3 normalMatrix;

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 TextureCoords0;

out vec3 N;
out vec3 eyeCoords;
out vec2 texCoords0;

void main()  {
    texCoords0 = TextureCoords0;

    N  = normalize(normalMatrix * VertexNormal);

    vec4 eyeCoordsPosition= modelViewMatrix * vec4(VertexPosition,1.0);
    eyeCoords = vec3(eyeCoordsPosition);

    gl_Position = projectionMatrix * eyeCoordsPosition;

}
