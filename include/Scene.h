#pragma once
#include "Global.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"

class Scene {
    public:
        ~Scene();
        Scene();
        Scene(DirLight* _dirLight, PointLight* _pointLight);
        int addCamera(Camera* camera);
        int addModel(Model *model);
        void setLight(DirLight* _dirLight, PointLight* _pointLight);
        DirLight *getDirLight();
        PointLight* getPointLight();
        const vector<Camera*>& getCameras();
        const vector<Model*>& getObjects();
    private:
        DirLight* dirLight;
        PointLight* pointLight;
        vector<Model*> objects;
        vector<Camera*> cameras;
};