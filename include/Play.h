// 控制仿真部分
#pragma once
#include "Global.h"
#include "Scene.h"


class Play {
    public:
        Play(Scene* scene);
        void control();
    private:
        Scene* scene;
};
