#pragma once
#include "Global.h"
#include "Shader.h"

struct Texture {
    unsigned int id;
    int width, height;
    string type;
    string name;  // 我们储存纹理的路径用于与其它纹理进行比较
    void transTexture(Shader* shader, int channel = 0) const;

    static Texture TextureFromFile(string path, string name, string type = "textureDiffuse");
    static Texture CubeTextureFromFile(vector<string> path, string name, string type = "cubeTexture");
    static Texture CubeTextureForFramebufferDepth(string name, int width, int height, string type = "cubeTexture");
    static Texture TextureForFramebufferColor(string name, int width, int height, string type = "textureDiffuse");
    static Texture TextureForFramebufferDepth(string name, int width, int height, string type = "textureDiffuse");
    static Texture TextureForFramebufferDepthStencil(string name, int width, int height, string type = "textureDiffuse");
};
