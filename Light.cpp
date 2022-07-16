#include "Light.h"


glm::mat4 DirLight::getLightViewMatrix() {
    return lightCamera->getProjectionMatrix() * lightCamera->getViewMatrix();
}

void DirLight::transLight(const string &name, ShaderPtr shader) {
    shader->setVec3("dirLight" + name + ".ambient", ambient);
    shader->setVec3("dirLight" + name + ".diffuse", diffuse);
    shader->setVec3("dirLight" + name + ".specular", specular);
    shader->setVec3("dirLight" + name + ".direction", direction);
}

void DirLight::transLightCamera(ShaderPtr shader) {
	shader->setMat4("shadowMatrice", getLightViewMatrix());
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

void PointLight::transLightCamera(ShaderPtr shader) {
	auto shadowMatrices = getLightViewMatrix();
	for(int i = 0; i < 6; i++) {
		shader->setMat4("shadowMatrices[" + to_string(i) + "]", shadowMatrices[i]);
	}
	shader->setVec3("lightPos", lightCamera->eye);
	shader->setFloat("far_plane", lightCamera->far);
}

PointLight::PointLight() {
    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;
	ambient = glm::vec3(0.2f);
    diffuse = glm::vec3(0.6f);
    specular = glm::vec3(0.2f);
    position = glm::vec3(0, 30, 0);
	lightCamera = make_shared<Camera>(PERSPECTIVE);
	lightCamera->aspect = 1;
	lightCamera->eye = position;
	lightCamera->fov = 90;
	lightCamera->far = 100;
	lightCamera->near = 1.5;
}

vector<glm::mat4> PointLight::getLightViewMatrix() {
	vector<glm::mat4> shadowTransforms;
	glm::mat4 shadowProj = lightCamera->getProjectionMatrix();

	lightCamera->up = glm::vec3(0.0,-1.0,0.0);
	lightCamera->dir = glm::vec3(1.0,0.0,0.0);
	shadowTransforms.push_back(shadowProj * lightCamera->getViewMatrix());

	lightCamera->up = glm::vec3(0.0,-1.0,0.0);
	lightCamera->dir = glm::vec3(-1.0,0.0,0.0);
	shadowTransforms.push_back(shadowProj * lightCamera->getViewMatrix());
	
	lightCamera->up = glm::vec3(0.0,0.0,1.0);
	lightCamera->dir = glm::vec3(0.0,1.0,0.0);
	shadowTransforms.push_back(shadowProj * lightCamera->getViewMatrix());

	lightCamera->up = glm::vec3(0.0,0.0,-1.0);
	lightCamera->dir = glm::vec3(0.0,-1.0,0.0);
	shadowTransforms.push_back(shadowProj * lightCamera->getViewMatrix());
	
	lightCamera->up = glm::vec3(0.0,-1.0,0.0);
	lightCamera->dir = glm::vec3(0.0,0.0,1.0);
	shadowTransforms.push_back(shadowProj * lightCamera->getViewMatrix());
	
	lightCamera->up = glm::vec3(0.0,-1.0,0.0);
	lightCamera->dir = glm::vec3(0.0,0.0,-1.0);
	shadowTransforms.push_back(shadowProj * lightCamera->getViewMatrix());

	return shadowTransforms;
}

DirLight::DirLight() {
	ambient = glm::vec3(0.5f);
    diffuse = glm::vec3(0.6f);
    specular = glm::vec3(0.2f);
    direction = glm::normalize(glm::vec3(0.5, -1, 0));
	lightCamera = make_shared<Camera>(ORTHO);
	lightCamera->dir = direction;
	lightCamera->scale = 50;
	lightCamera->eye = glm::vec3(0, 50, 0);
}

void PointLight::transLight(const string &name, ShaderPtr shader) {
    shader->setVec3( "pointLight" + name + ".ambient", ambient);
    shader->setVec3( "pointLight" + name + ".diffuse", diffuse);
    shader->setVec3( "pointLight" + name + ".specular", specular);
    shader->setVec3( "pointLight" + name + ".position", position);
    shader->setFloat("pointLight" + name + ".constant", constant);
    shader->setFloat("pointLight" + name + ".linear", linear);
    shader->setFloat("pointLight" + name + ".quadratic",quadratic);
    shader->setFloat("pointLight" + name + ".farPlane",lightCamera->far);
}

glm::vec3 PointLight::doMovement() {
	extern unsigned char KEYBUFFER[1024];
	extern float frameTime;
	float cameraSpeed = 5.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	
	if(KEYBUFFER['w']) translation.x += cameraSpeed;
  	if(KEYBUFFER['s']) translation.x -= cameraSpeed;
  	if(KEYBUFFER['a']) translation.y += cameraSpeed;
  	if(KEYBUFFER['d']) translation.y -= cameraSpeed;
	if(KEYBUFFER[' ']) translation.z += cameraSpeed;
	if(KEYBUFFER['q']) translation.z -= cameraSpeed;
	
	position += translation;
	lightCamera->eye = position;
	return translation;
}

glm::vec3 DirLight::doMovement() {
	extern unsigned char KEYBUFFER[1024];
	extern float frameTime;
	float cameraSpeed = 5.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	if(KEYBUFFER['w']) translation.z += cameraSpeed;
  	if(KEYBUFFER['s']) translation.z -= cameraSpeed;
  	if(KEYBUFFER['a']) translation.x += cameraSpeed;
  	if(KEYBUFFER['d']) translation.x -= cameraSpeed;
	direction += translation;
	direction = glm::normalize(direction);
	lightCamera->dir = direction;
	return translation;
}

