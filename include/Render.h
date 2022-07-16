#pragma once
#include "Global.h"
#include "Scene.h"
#include "Shader.h"
#include "Framebuffer.h"



class Render {
public:
    Render(ScenePtr scene);
    void masterRender();
    void sceneRender();
    void renderSkybox();
    void renderDepthMap();
    void renderCubeDepthMap();
    ScenePtr scene;

    ShaderPtr masterShader;
    ShaderPtr depthShader; 
    ShaderPtr skyboxShader; 
    ShaderPtr basicShader;
    ShaderPtr cubedepthShader;
    
    FrameBufferPtr glassFbo;
    FrameBufferPtr depthFbo;
}; 


