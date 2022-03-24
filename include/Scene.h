#pragma once
#include "Global.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"

class    Scene {
    public:
        ~Scene();
        Scene();
        DirLight* dirLight;
        vector<PointLight*> pointLights;
        map<string, Camera*> cameras;
        map<string, Model*> objects;
};