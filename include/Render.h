#pragma once
#include "Global.h"
#include "Scene.h"
#include "Shader.h"
#include "Toolbox.h"



class Render {
public:
    Render(ScenePtr scene);
    void masterRender();
    void sceneRender();
    void renderGlass();
    void renderLightCube();
    void renderSkybox();
    void renderDepthMap();
    void renderCubeDepthMap();
    void renderLine(glm::vec3 p1, glm::vec3 normal,glm::mat4 model = glm::mat4(1.0), int lineWide = 1, glm::vec3 color = glm::vec3(1.0, 0, 0));
    void debugRender();
    void renderDoorEntity(DoorType doorType);
    void renderDoor(int mx, CameraPtr faCamera, DoorType doorType, int cur = 1);

    ScenePtr scene;

    ShaderPtr debugShader;
    ShaderPtr masterShader;
    ShaderPtr depthShader; 
    ShaderPtr skyboxShader; 
    ShaderPtr basicShader;
    ShaderPtr glassShader;
    ShaderPtr cubedepthShader;
    
    FrameBufferPtr glassFbo;
    FrameBufferPtr depthFbo;
}; 


