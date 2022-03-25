#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexture;

out vec3 fPosition;
out vec3 fNormal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out VS_OUT {
    vec2 texCoords;
} vs_out;

void main() {
    fPosition = vec3(model * vec4(vPosition, 1.0));
    fNormal = normalize(vec3(transpose(inverse(model)) * vec4(vNormal, 0))); // 法线要也要和顶点一起变换，如果scale放大缩小后，还要单位化 //TODO 证明，归一化问题
    vs_out.texCoords = vTexture;
    gl_Position = projection * view * vec4(fPosition, 1.0); // 此处不要用透视除法，如果提前进行透视除法，裁剪会出错。
}
