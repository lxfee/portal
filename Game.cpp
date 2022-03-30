#include "Game.h"

Game::Game(Scene* scene) : scene(scene) {}

void Game::idle() {
    scene->masterCamera->doMovement();
}

void Game::mouseWheel(int button, int dir, int x, int y) {
    scene->masterCamera->mouseWheel(button, dir, x, y);
}

void Game::mouseMotion(float mouseDeltaX, float mouseDeltaY) {
    scene->masterCamera->mouseMotion(mouseDeltaX, mouseDeltaY);
}