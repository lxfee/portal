#pragma once

#include "Global.h"
#include "Scene.h"

class Render {
    public:
        ~Render();
        Render(Scene* scene);
        void render();
        
    private:
        Scene* scene;
        map<string, Shader*> shaders;
        Model* floor; // 临时使用      
};
