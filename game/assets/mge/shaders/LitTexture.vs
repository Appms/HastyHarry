//DIFFUSE COLOR VERTEX SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 MVP;

out vec2 fragTexCoord;
out vec3 fragVert;
out vec3 fragNormal;

void main( void ){
    fragTexCoord = uv;
    fragNormal = normal;
    fragVert = vertex;

    gl_Position = MVP * vec4( vertex, 1 );
}
