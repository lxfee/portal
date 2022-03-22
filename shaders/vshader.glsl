#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexture;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexture;

uniform mat4 vp;

void main() {
    vec4 position = vp * vec4(vPosition, 1.0);
    fPosition = position.xyz / position.w;
    fNormal = vNormal;
    fTexture = vTexture;
}