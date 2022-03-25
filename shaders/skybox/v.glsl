#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexture;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

uniform mat4 model;

void main() {
    TexCoords = vec3(model * vec4(vPosition, 1.0));
    gl_Position = projection * view * vec4(TexCoords, 1.0);
}