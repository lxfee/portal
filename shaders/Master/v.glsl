#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexture;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 dirLightViewMatrix;


out VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 position;
    vec4 dirLightFpos;      // 平行光视角
} vs_out;

void main() {
    vs_out.position = vec3(model * vec4(vPosition, 1.0));
    vs_out.normal = transpose(inverse(mat3(model))) * vNormal; // 法线要也要和顶点一起变换，如果scale放大缩小后，还要单位化 //TODO 证明，归一化问题
    vs_out.texCoords = vTexture;
    vs_out.dirLightFpos = dirLightViewMatrix * vec4(vs_out.position, 1.0);
    gl_Position = projection * view * vec4(vs_out.position, 1.0); // 此处不要用透视除法，如果提前进行透视除法，裁剪会出错。
}
