#pragma once
#include "Global.h"
#include "Model.h"
#include "Camera.h"
#include "Toolbox.h"
enum DoorType {Door1 = 0, Door2 = 1};

class Portal {
public:
    Portal();
    glm::mat4 getDoorModel(DoorType doorType);
    glm::vec4 getDoorPannel(DoorType doorType);
    void updateDoorCamera(CameraPtr masterCamera, CameraPtr doorCamera, DoorType doorType);
    Model *door1, *door2;
};

typedef shared_ptr<Portal> PortalPtr;
