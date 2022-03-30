#version 330 core

out vec4 fColor;
in vec2 texCoords;

// uniform sampler2D dirDepMap;
uniform sampler2D sceneC;

float Shift(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC 
    return z;    
}


void main() {
    // float depValue = texture(dirDepMap, texCoords).r;
    // depValue = exp(20 * Shift(depValue)) / exp(20.0); // 用于点光源距离调试
    // fColor = vec4(vec3(depValue), 1.0);
    // fColor = texture(dirDepMap, texCoords);
    fColor = texture(sceneC, texCoords);
}