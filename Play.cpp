#include "Play.h"

Play::Play(Scene* scene) : scene(scene) {
	for(int i = 0; i < sizeof KEYBUFFER; i++) KEYBUFFER[i] = false;
}

void Play::idle() {
	static enum{CAMERA, LIGHT, DEBUG} status = CAMERA;

	if(KEYBUFFER['1']) {
		status = CAMERA;
	}
	if(KEYBUFFER['2']) {
		status = LIGHT;
	}
	if(KEYBUFFER['3']) {
		status = DEBUG;
	}
	
	switch(status) {
		case CAMERA: 
			scene->cameras["camera"] ->doMovement(KEYBUFFER); 
			break;
		case LIGHT: 
			scene->dirLight->doMovement(KEYBUFFER); 
			break;
		case DEBUG: 
			scene->cameras["sCamera"]->doMovement(KEYBUFFER);
			break; 
	}
	scene->cameras["sCamera"]->dir = normalize(scene->dirLight->direction);
}

void Play::keyboard(unsigned char key, int x, int y, int action) {
	
}

void Play::mouseMotion(float deltaX, float deltaY) {
	auto camera = scene->cameras["camera"];
	camera->mouseMotion(deltaX, deltaY);
}


void Play::mouseWheel(int button, int dir, int x, int y) {
	auto camera = scene->cameras["camera"];
	camera->mouseWheel(button, dir, x, y);
}