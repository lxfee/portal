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


void main() {             
    fColor = texture(material.textureDiffuse[0], texCoords);
}