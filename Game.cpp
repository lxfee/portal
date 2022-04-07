#include "Game.h"

Game::Game(Scene* scene) : scene(scene) {}

void Game::idle() {
    scene->masterCamera->doMovement();
    scene->steve->doMovement();
    float dis = -glm::dot(scene->portal->getDoorPannel(Door2), glm::vec4(scene->steve->eye->eye, 1.0f));
    if(dis >= 0 && dis < 0.3) scene->steve->position = glm::vec3(scene->portal->getDoorModel(Door1) * glm::vec4(scene->steve->position, 1.0f));
}

void Game::mouseWheel(int button, int dir, int x, int y) {
    // scene->masterCamera->mouseWheel(button, dir, x, y);
}

void Game::mouseMotion(float mouseDeltaX, float mouseDeltaY) {
    scene->masterCamera->mouseMotion(mouseDeltaX, mouseDeltaY);
    scene->steve->mouseMotion(mouseDeltaX, mouseDeltaY);
}