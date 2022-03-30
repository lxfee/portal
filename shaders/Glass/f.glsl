#version 330 core

out vec4 fColor;

uniform vec3 color;
in vec4 texPos;
in vec2 texCoords;


struct Material {
    int diffuseNum;
    int specularNum;
    sampler2D textureDiffuse[4];
    sampler2D textureSpecular[4];
};


vec2 getTexPos(vec4 tPos) {
    vec3 projCoords = tPos.xyz / tPos.w;
    projCoords = projCoords * 0.5 + 0.5;
    return projCoords.xy;
}

uniform Material material;
uniform sampler2D sceneC;

void main() {
    vec4 sceneColor = texture(sceneC, getTexPos(texPos));
    vec4 texColor = texture(material.textureDiffuse[0], texCoords);
    fColor = mix(sceneColor, texColor, 0.4);
}
