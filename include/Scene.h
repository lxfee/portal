#pragma once
#include "Global.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Steve.h"
#include "Portal.h"

class Scene {
    public:
        ~Scene();
        Scene();
        /*--------------------------光源--------------------------*/
        DirLight* dirLight;
        vector<PointLight*> pointLights;

        /*--------------------------相机-------------------------*/
        Camera* masterCamera;
        Camera* doorCamera;

        /*--------------------------模型-------------------------*/
        vector<Model*> objects; // 普通物体
        vector<Model*> glasses; // 玻璃材质物体
        Model* floor;           // 地板
        Steve* steve;           // 玩家人物

        Model* cube;            // 方形物体
        Model* debugPannel;     // 调试面板
        Model* skybox;          // 天空盒
        Portal* portal;
        Model* lamp;

        /*--------------------------纹理--------------------------*/
        Texture TDirDepth;      // 平行光阴影
        Texture TPointDepth;    // 点光源阴影
        Texture TS, TC;         // 用于帧缓冲
        Texture Tskybox;        // 立方体贴图，用于天空盒

        

};