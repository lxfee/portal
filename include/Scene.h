#pragma once
#include "Global.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"

class Scene {
    public:
        ~Scene();
        Scene();
        vector<PointLight*> pointLights;
        vector<Camera*> cameras;
        vector<Model*> objects;
};