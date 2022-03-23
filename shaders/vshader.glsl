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
    gl_Position = position; // 此处不要用透视除法，如果提前进行透视除法，会出错。

    fPosition = vPosition;
    fNormal = vNormal;
    fTexture = vTexture;
}
