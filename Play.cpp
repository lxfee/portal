#include "Play.h"

Play::Play(Scene* scene) : scene(scene) {}

void Play::keyboard(unsigned char key, int x, int y, int action) {
	auto camera = scene->getCameras().front();
    switch (key) {
		default:
			camera->keyboard(key, x, y, action); break;
	}
}

void Play::mouseMotion(float deltaX, float deltaY) {
	auto camera = scene->getCameras().front();
	camera->mouseMotion(deltaX, deltaY);
}


void Play::mouseWheel(int button, int dir, int x, int y) {
	auto camera = scene->getCameras().front();
	camera->mouseWheel(button, dir, x, y);
}