#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexture;

uniform mat4 shadowMatrice;
uniform mat4 model;

void main() {
    gl_Position = shadowMatrice * model * vec4(vPosition, 1.0f);
}