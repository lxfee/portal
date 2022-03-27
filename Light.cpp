#include "Light.h"

Light::~Light() {
	delete lightCamera;
}

glm::mat4 Light::getLightViewMatrix() {
    return lightCamera->getProjectionMatrix() * lightCamera->getViewMatrix();
}

void DirLight::transLight(const string &name, Shader* shader) {
    shader->setVec3("dirLight" + name + ".ambient", ambient);
    shader->setVec3("dirLight" + name + ".diffuse", diffuse);
    shader->setVec3("dirLight" + name + ".specular", specular);
    shader->setVec3("dirLight" + name + ".direction", direction);
}

void DirLight::setDirection(glm::vec3 direction) {
	lightCamera->dir = direction;
	this->direction = direction;
}

glm::vec3 DirLight::getDirection() {
	return direction;
}

void PointLight::setPosition(glm::vec3 position) {
	lightCamera->eye = position;
	this->position = position;
}

glm::vec3 PointLight::getPosition() {
	return position;
}


PointLight::PointLight() {
    constant = 1.0f;
    linear = 0.007f;
    quadratic = 0.0002f;
	ambient = glm::vec3(0.5);
    diffuse = glm::vec3(0.6);
    specular = glm::vec3(0.2);
    position = glm::vec3(0, 30, 0);
	lightCamera = new Camera(PERSPECTIVE);
	lightCamera->dir = glm::vec3(0, -1, 0);
	lightCamera->aspect = 1;
	lightCamera->eye = position;
	lightCamera->fov = 120;
}

DirLight::DirLight() {
	ambient = glm::vec3(0.5);
    diffuse = glm::vec3(0.6);
    specular = glm::vec3(0.2);
    direction = glm::vec3(0, -1, 0);
	lightCamera = new Camera(ORTHO);
	lightCamera->dir = direction;
	lightCamera->scale = 50;
	lightCamera->eye = glm::vec3(0, 50, 0);
}

void PointLight::transLight(const string &name, Shader* shader) {
    shader->setVec3( "pointLight" + name + ".ambient", ambient);
    shader->setVec3( "pointLight" + name + ".diffuse", diffuse);
    shader->setVec3( "pointLight" + name + ".specular", specular);
    shader->setVec3( "pointLight" + name + ".position", position);
    shader->setFloat("pointLight" + name + ".constant", constant);
    shader->setFloat("pointLight" + name + ".linear", linear);
    shader->setFloat("pointLight" + name + ".quadratic",quadratic);
}

glm::vec3 PointLight::doMovement(unsigned char* KEYBUFFER) {
	extern float frameTime;
	float cameraSpeed = 5.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	
	if(KEYBUFFER[KEY_W]) translation.x += cameraSpeed;
  	if(KEYBUFFER[KEY_S]) translation.x -= cameraSpeed;
  	if(KEYBUFFER[KEY_A]) translation.y += cameraSpeed;
  	if(KEYBUFFER[KEY_D]) translation.y -= cameraSpeed;
	if(KEYBUFFER[' ']) translation.z += cameraSpeed;
	if(KEYBUFFER['q']) translation.z -= cameraSpeed;
	
	position += translation;
	lightCamera->eye = position;
	return translation;
}

glm::vec3 DirLight::doMovement(unsigned char* KEYBUFFER) {
	extern float frameTime;
	float cameraSpeed = 5.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	if(KEYBUFFER[KEY_W]) translation.x += cameraSpeed;
  	if(KEYBUFFER[KEY_S]) translation.x -= cameraSpeed;
  	if(KEYBUFFER[KEY_A]) translation.z += cameraSpeed;
  	if(KEYBUFFER[KEY_D]) translation.z -= cameraSpeed;
	direction += translation;
	direction = glm::normalize(direction);
	lightCamera->dir = direction;
	return translation;
}

