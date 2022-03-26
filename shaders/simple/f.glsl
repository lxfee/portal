#version 330 core

out vec4 fColor;
in vec2 texCoords;

struct Material {
    int diffuseNum;
    int specularNum;
    sampler2D textureDiffuse[4];
    sampler2D textureSpecular[4];
};

uniform Material material;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC 
    return z;    
}


void main() {

    float depValue = exp(20 * LinearizeDepth(texture(material.textureDiffuse[0], texCoords).r)) / exp(20.0);
    fColor += vec4(vec3(depValue), 1.0);
}