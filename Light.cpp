#include "Light.h"

DirLight::DirLight() {
	ambient = glm::vec3(0.5f);
    diffuse = glm::vec3(0.6f);
    specular = glm::vec3(0.2f);
    direction = glm::normalize(glm::vec3(0.5, -1, 0));
	scale = 100;
	position = glm::vec3(0, 50, 0);
	viewnear = 1;
	viewfar = 100;
	updateLightViewMatrix();
}


void DirLight::transLight(const string &name, ShaderPtr shader) {
    shader->setVec3("dirLight" + name + ".ambient", ambient);
    shader->setVec3("dirLight" + name + ".diffuse", diffuse);
    shader->setVec3("dirLight" + name + ".specular", specular);
    shader->setVec3("dirLight" + name + ".direction", direction);
}

void DirLight::updateLightViewMatrix() {
	glm::mat4 lookat = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 orthmat = glm::ortho(-scale, scale, -scale, scale, viewnear, viewfar);
	lightViewMatrix = orthmat * lookat;
}

void DirLight::transLightCamera(ShaderPtr shader) {
	shader->setMat4("dirLightViewMatrix", lightViewMatrix);
}

void DirLight::setDirection(glm::vec3 direction) {
	this->direction = direction;
	updateLightViewMatrix();
}

void PointLight::setPosition(glm::vec3 position) {
	this->position = position;
	updateLightViewMatrix();
}

void PointLight::transLightCamera(ShaderPtr shader) {
	for(int i = 0; i < 6; i++) {
		shader->setMat4("pointLightViewMatrixs[" + to_string(i) + "]", lightViewMatrixs[i]);
	}
	shader->setVec3("lightPos", position);
	shader->setFloat("far_plane", viewfar);
}

PointLight::PointLight() {
    constant = 1.0f;
    linear = 0.05f;
    quadratic = 0.012f;
	
	ambient = glm::vec3(0.2f);
    diffuse = glm::vec3(0.6f);
    specular = glm::vec3(0.2f);
    position = glm::vec3(0, 30, 0);

	viewfar = 100;
	viewnear = 1.5;

	lightViewMatrixs.resize(6);
	updateLightViewMatrix();
}


void PointLight::updateLightViewMatrix() {
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, viewnear, viewfar);
	lightViewMatrixs[0] = (shadowProj * glm::lookAt(position, glm::vec3(1.0,0.0,0.0) + position, glm::vec3(0.0,-1.0,0.0)));
	lightViewMatrixs[1] = (shadowProj * glm::lookAt(position, glm::vec3(-1.0,0.0,0.0) + position, glm::vec3(0.0,-1.0,0.0)));
	lightViewMatrixs[2] = (shadowProj * glm::lookAt(position, glm::vec3(0.0,1.0,0.0) + position, glm::vec3(0.0,0.0,1.0)));
	lightViewMatrixs[3] = (shadowProj * glm::lookAt(position, glm::vec3(0.0,-1.0,0.0) + position, glm::vec3(0.0,0.0,-1.0)));
	lightViewMatrixs[4] = (shadowProj * glm::lookAt(position, glm::vec3(0.0,0.0,1.0) + position, glm::vec3(0.0,-1.0,0.0)));
	lightViewMatrixs[5] = (shadowProj * glm::lookAt(position, glm::vec3(0.0,0.0,-1.0) + position, glm::vec3(0.0,-1.0,0.0)));
}

void PointLight::transLight(const string &name, ShaderPtr shader) {
    shader->setVec3( "pointLight" + name + ".ambient", ambient);
    shader->setVec3( "pointLight" + name + ".diffuse", diffuse);
    shader->setVec3( "pointLight" + name + ".specular", specular);
    shader->setVec3( "pointLight" + name + ".position", position);
    shader->setFloat("pointLight" + name + ".constant", constant);
    shader->setFloat("pointLight" + name + ".linear", linear);
    shader->setFloat("pointLight" + name + ".quadratic",quadratic);
    shader->setFloat("pointLight" + name + ".farPlane",viewfar);
}
