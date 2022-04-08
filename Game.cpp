#include "Game.h"

Game::Game(Scene* scene) : scene(scene) {}

void Game::idle() {
    // scene->masterCamera->doMovement();
    extern unsigned char KEYBUFFER[1024];
    extern unsigned char KEYBUFFER[1024];
	extern float frameTime;
	float speed = 10.0f * (frameTime / 1000);

    static int control = 0;
    if(KEYBUFFER['-']) {
        control = 1;
    } 
    if(KEYBUFFER['=']){
        control = 2;
    }
    if(KEYBUFFER['0']) {
        control = 0;
    }
    if(control == 0) {
    scene->steve->doMovement();
    } if(control == 1){
        // 移动传送门

  	    glm::vec3 translation(0);
	    glm::vec3 dir = normalize(scene->masterCamera->dir);
	    glm::vec3 up = glm::vec3(0, 1.0, 0);
	    glm::vec3 right = glm::normalize(glm::cross(up, dir));
	    glm::vec3 front = glm::normalize(glm::cross(right, up));

	    if(KEYBUFFER['w']) translation += front * speed;
  	    if(KEYBUFFER['s']) translation -= front * speed;
  	    if(KEYBUFFER['a']) translation += right * speed;
  	    if(KEYBUFFER['d']) translation -= right * speed;
	    if(KEYBUFFER[' ']) translation += up * speed;
	    if(KEYBUFFER['q']) translation -= up * speed;

	    scene->portal->door1->translation += translation;

        if(KEYBUFFER['1']) scene->portal->door1->rotation.x += 10 * speed;
	    if(KEYBUFFER['2']) scene->portal->door1->rotation.x -= 10 * speed;
	    if(KEYBUFFER['3']) scene->portal->door1->rotation.y += 10 * speed;
	    if(KEYBUFFER['4']) scene->portal->door1->rotation.y -= 10 * speed;
	    if(KEYBUFFER['5']) scene->portal->door1->rotation.z += 10 * speed;
	    if(KEYBUFFER['6']) scene->portal->door1->rotation.z -= 10 * speed;

    } else if(control == 2){
        // 移动传送门

        glm::vec3 translation(0);
	    glm::vec3 dir = normalize(scene->masterCamera->dir);
	    glm::vec3 up = glm::vec3(0, 1.0, 0);
	    glm::vec3 right = glm::normalize(glm::cross(up, dir));
	    glm::vec3 front = glm::normalize(glm::cross(right, up));

	    if(KEYBUFFER['w']) translation += front * speed;
  	    if(KEYBUFFER['s']) translation -= front * speed;
  	    if(KEYBUFFER['a']) translation += right * speed;
  	    if(KEYBUFFER['d']) translation -= right * speed;
	    if(KEYBUFFER[' ']) translation += up * speed;
	    if(KEYBUFFER['q']) translation -= up * speed;

	    scene->portal->door2->translation += translation;

	    if(KEYBUFFER['1']) scene->portal->door2->rotation.x += 10 * speed;
	    if(KEYBUFFER['2']) scene->portal->door2->rotation.x -= 10 * speed;
	    if(KEYBUFFER['3']) scene->portal->door2->rotation.y += 10 * speed;
	    if(KEYBUFFER['4']) scene->portal->door2->rotation.y -= 10 * speed;
	    if(KEYBUFFER['5']) scene->portal->door2->rotation.z += 10 * speed;
	    if(KEYBUFFER['6']) scene->portal->door2->rotation.z -= 10 * speed;
    }
    
}

void Game::mouseWheel(int button, int dir, int x, int y) {
    scene->masterCamera->mouseWheel(button, dir, x, y);
}

void Game::mouseMotion(float mouseDeltaX, float mouseDeltaY) {
    scene->masterCamera->mouseMotion(mouseDeltaX, mouseDeltaY);
    scene->steve->mouseMotion(mouseDeltaX, mouseDeltaY);
}