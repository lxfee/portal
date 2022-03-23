// 控制仿真部分
#pragma once
#include "Global.h"
#include "Scene.h"
	
class Play {
    public:
        Play(Scene* scene);
        // 键盘操作
        void keyboard(unsigned char key, int x, int y, int action);
        void mouseMotion(float deltaX, float deltaY);
	    void mouseWheel(int button, int dir, int x, int y);
        void idle();
    private:
        Scene* scene;
        unsigned char KEYBUFFER[1024];
};
