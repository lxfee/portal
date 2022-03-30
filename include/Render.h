#pragma once
#include "Global.h"
#include "Scene.h"
#include "Shader.h"
#include "Toolbox.h"


class Render {
public:
    Render(Scene* scene);
    void masterRender();
    void sceneRender(const FrameBuffer* fbo = nullptr);
    void renderGlass();
    void renderLightCube();
    void renderLine(glm::vec3 p1, glm::vec3 p2, int lineWide = 1);
    void debugRender();
    

    Scene* scene;

    Shader* debugShader;
    Shader* masterShader;
    Shader* depthShader; 
    Shader* skyboxShader; 
    Shader* basicShader;
    Shader* glassShader;
    
    FrameBuffer* glassFbo;
    FrameBuffer* depthFbo;
};