#pragma once
#include "Global.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"

class Scene {
    public:
        Scene();
        /*--------------------------Lights--------------------------*/
        DirLightPtr dirLight;
        vector<PointLightPtr> pointLights;

        /*--------------------------Camera-------------------------*/
        CameraPtr masterCamera;
        CameraPtr doorCamera;

        /*--------------------------Models-------------------------*/
        vector<ModelPtr> objects;
        ModelPtr floor;
        ModelPtr lamp;

        ModelPtr cube;
        ModelPtr skybox;

        /*--------------------------Texture--------------------------*/
        Texture TDirDepth;
        Texture TPointDepth;
        Texture TS, TC;         // For framebuffer
        Texture Tskybox;        // For skybox
};

typedef shared_ptr<Scene> ScenePtr;