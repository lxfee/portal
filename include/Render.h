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
    void renderLine(glm::vec3 p1, glm::vec3 p2,glm::mat4 model = glm::mat4(1.0), int lineWide = 1, glm::vec3 color = glm::vec3(1.0, 0, 0));
    void debugRender();
    void renderdoorB();
    void renderdoorR();

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