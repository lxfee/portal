#pragma once

#include "Global.h"
#include "Scene.h"

class Render {
    public:
        Render(Scene* scene);
        void render();
    private:
        Scene* scene;
};
