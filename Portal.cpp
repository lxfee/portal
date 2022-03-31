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
	
	door1->translation = glm::vec3(0, 5.0, 0.0);
	door2->translation = glm::vec3(0, 5.0, 5.0);
}


glm::mat4 Portal::getDoor1Model() {
    auto model = door2->getModelMatrix() * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::inverse(door1->getModelMatrix());
    return model;
}
glm::mat4 Portal::getDoor2Model() {
    auto model = door1->getModelMatrix() * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::inverse(door2->getModelMatrix());
    return model;
}

glm::vec4 Portal::getDoor1Pannel() {
    return getPannel(glm::vec3(0), glm::vec3(0, 0, -1), door1->getModelMatrix());
}
glm::vec4 Portal::getDoor2Pannel() {
    return getPannel(glm::vec3(0), glm::vec3(0, 0, -1), door2->getModelMatrix());
}

void Portal::updateDoor1Camera(Camera* masterCamera, Camera* doorCamera) {
    *(doorCamera->pannel) = getDoor1Pannel();
    auto p1 = masterCamera->eye;
	auto p2 = masterCamera->eye + masterCamera->dir;
    auto model = getDoor1Model();
	p1 = glm::vec3(model * glm::vec4(p1, 1.0));
	p2 = glm::vec3(model * glm::vec4(p2, 1.0));
	auto dir = glm::normalize(p2 - p1);
	doorCamera->dir = dir;
	doorCamera->eye = p1;
}


void Portal::updateDoor2Camera(Camera* masterCamera, Camera* doorCamera) {
    *(doorCamera->pannel) = getDoor2Pannel();
    auto p1 = masterCamera->eye;
	auto p2 = masterCamera->eye + masterCamera->dir;
    auto model = getDoor2Model();
	p1 = glm::vec3(model * glm::vec4(p1, 1.0));
	p2 = glm::vec3(model * glm::vec4(p2, 1.0));
	auto dir = glm::normalize(p2 - p1);
	doorCamera->dir = dir;
	doorCamera->eye = p1;
}