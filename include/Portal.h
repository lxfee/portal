#pragma once
#include "Global.h"
#include "Model.h"
#include "Camera.h"
#include "Toolbox.h"

class Portal {
public:
    Portal();
    glm::mat4 getDoor1Model();
    glm::vec4 getDoor1Pannel();
    void updateDoor1Camera(Camera* masterCamera, Camera* doorCamera);

    glm::mat4 getDoor2Model();
    glm::vec4 getDoor2Pannel();
    void updateDoor2Camera(Camera* masterCamera, Camera* doorCamera);

    
    Model* door1, *door2;
    
};
