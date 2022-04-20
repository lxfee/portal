#pragma once
#include "Global.h"
#include "Scene.h"

class Game {
public:
    Game(ScenePtr scene);
    void idle();
    void mouseWheel(int button, int dir, int x, int y);
    void mouseMotion(float mouseDeltaX, float mouseDeltaY);
private:
    ScenePtr scene;
};
