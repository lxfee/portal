#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexture;
out vec4 fColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

uniform Material material;


void main() {
    texture2D(material.texture_diffuse1, fTexture);
	fColor = vec4(1.0,1.0,1.0,1.0);
}

