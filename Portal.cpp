#include "Portal.h"

Portal::Portal() {
    vector<Vertex> doorInfo = {
		{glm::vec3( 0.5,  0.5, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5,  0.5, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(-0.5, -0.5, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5, -0.5, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
	};
    door1 = new Model({Mesh(doorInfo, {0, 1, 2, 2, 3, 0}, {})}); 
    door2 = new Model({Mesh(doorInfo, {0, 1, 2, 2, 3, 0}, {})}); 
	
	door1->translation = glm::vec3(-1.0, 4, 5);
	door2->translation = glm::vec3(2.0, 4, -1);
	door1->scale = glm::vec3(6.0, 8.0, 6.0);
	door2->scale = glm::vec3(6.0, 8.0, 6.0);
	door1->rotation.y = 0;
	door2->rotation.y = 90;
}


glm::mat4 Portal::getDoorModel(DoorType doorType) {
	switch(doorType) {
		case Door1:
    		return door2->getModelMatrix() * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::inverse(door1->getModelMatrix());
		case Door2:
			return door1->getModelMatrix() * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::inverse(door2->getModelMatrix());
	}
}


glm::vec4 Portal::getDoorPannel(DoorType doorType) {
	switch(doorType) {
		case Door2:
    		return getPannel(glm::vec3(0), glm::vec3(0, 0, -1), door1->getModelMatrix());
		case Door1:
			return getPannel(glm::vec3(0), glm::vec3(0, 0, -1), door2->getModelMatrix());
	}
}

void Portal::updateDoorCamera(CameraPtr masterCamera, CameraPtr doorCamera, DoorType doorType) {
    if(doorCamera->pannel) *(doorCamera->pannel) = getDoorPannel(doorType);
    auto p1 = masterCamera->eye;
    auto normal = masterCamera->dir;
	auto up = masterCamera->up;
	auto model = getDoorModel(doorType);
	p1 = glm::vec3(model * glm::vec4(p1, 1.0));
	normal = glm::transpose(glm::inverse(glm::mat3(model))) * normal;
	up = glm::transpose(glm::inverse(glm::mat3(model))) * up;
	doorCamera->dir = normal;
	doorCamera->eye = p1;
	doorCamera->up = up;
	doorCamera->fov = masterCamera->fov;
}
