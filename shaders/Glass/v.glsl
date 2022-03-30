#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexture;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec4 texPos;
out vec2 texCoords;

void main() {
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    texPos = projection * view * model * vec4(vPosition, 1.0);
    texCoords = vTexture;
}