#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube cubeTexture;

void main() {    
    FragColor = vec4(texture(cubeTexture, TexCoords).rgb, 1.0);
    gl_FragDepth = 0.999999;
}