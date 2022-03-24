#include "Play.h"

Play::Play(Scene* scene) : scene(scene) {
	for(int i = 0; i < sizeof KEYBUFFER; i++) KEYBUFFER[i] = false;
}

void Play::idle() {
	extern float frameTime;
	auto camera = scene->cameras.front();
	auto light = scene->dirLight;
	static enum{CAMERA, LIGHT} status = CAMERA;

	if(KEYBUFFER['1']) {
		status = CAMERA;
	}
	if(KEYBUFFER['2']) {
		status = LIGHT;
	}
	
	switch(status) {
		case CAMERA: camera->doMovement(KEYBUFFER); break;
		case LIGHT: light->doMovement(KEYBUFFER); break;
	}
}

void Play::keyboard(unsigned char key, int x, int y, int action) {
	if(action == GLFW_PRESS)
    	KEYBUFFER[key] = true;
	else if(action == GLFW_RELEASE)
    	KEYBUFFER[key] = false;
}

void Play::mouseMotion(float deltaX, float deltaY) {
	auto camera = scene->cameras.front();
	camera->mouseMotion(deltaX, deltaY);
}


void Play::mouseWheel(int button, int dir, int x, int y) {
	auto camera = scene->cameras.front();
	camera->mouseWheel(button, dir, x, y);
}