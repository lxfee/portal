#pragma once
#include "Global.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Steve.h"
#include "Portal.h"

class Scene {
    public:
        Scene();
        /*--------------------------光源--------------------------*/
        DirLightPtr dirLight;
        vector<PointLightPtr> pointLights;

        /*--------------------------相机-------------------------*/
        CameraPtr masterCamera;
        CameraPtr doorCamera;

        /*--------------------------模型-------------------------*/
        vector<ModelPtr> objects; // 普通物体
        vector<ModelPtr> glasses; // 玻璃材质物体
        ModelPtr floor;           // 地板
        StevePtr steve;           // 玩家人物

        ModelPtr cube;            // 方形物体
        ModelPtr debugPannel;     // 调试面板
        ModelPtr skybox;          // 天空盒
        PortalPtr portal;
        ModelPtr lamp;

        /*--------------------------纹理--------------------------*/
        Texture TDirDepth;      // 平行光阴影
        Texture TPointDepth;    // 点光源阴影
        Texture TS, TC;         // 用于帧缓冲
        Texture Tskybox;        // 立方体贴图，用于天空盒
};

typedef shared_ptr<Scene> ScenePtr;