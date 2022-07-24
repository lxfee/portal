#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexture;

uniform mat4 dirLightViewMatrix;
uniform mat4 model;

void main() {
    gl_Position = dirLightViewMatrix * model * vec4(vPosition, 1.0f);
}