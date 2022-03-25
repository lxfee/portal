#include "Light.h"


void DirLight::transLight(const string &name, Shader* shader) {
    shader->setVec3(name+".ambient", ambient);
    shader->setVec3(name+".diffuse", diffuse);
    shader->setVec3(name+".specular", specular);
    shader->setVec3(name+".direction", direction);
}

PointLight::PointLight() {
    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;
}

void PointLight::transLight(const string &name, Shader* shader) {
    shader->setVec3(name+".ambient", ambient);
    shader->setVec3(name+".diffuse", diffuse);
    shader->setVec3(name+".specular", specular);
    shader->setVec3(name+".position", position);
    shader->setFloat(name+".constant", constant);
    shader->setFloat(name+".linear", linear);
    shader->setFloat(name+".quadratic",quadratic);
}

void PointLight::doMovement(unsigned char* KEYBUFFER) {
	extern float frameTime;
	float cameraSpeed = 1.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	if(KEYBUFFER[GLFW_KEY_W]) 
	  	translation.x += cameraSpeed;
  	if(KEYBUFFER[GLFW_KEY_S])
	  	translation.x -= cameraSpeed;
  	if(KEYBUFFER[GLFW_KEY_A])
	  	translation.y += cameraSpeed;
  	if(KEYBUFFER[GLFW_KEY_D])
	  	translation.y -= cameraSpeed;
	if(KEYBUFFER[' ']) // 上
		translation.z += cameraSpeed;
	if(KEYBUFFER['q']) // 下
		translation.z -= cameraSpeed;
	position += translation;
}

void DirLight::doMovement(unsigned char* KEYBUFFER) {
	extern float frameTime;
	float cameraSpeed = 1.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	if(KEYBUFFER[GLFW_KEY_W]) 
	  	translation.x += cameraSpeed;
  	if(KEYBUFFER[GLFW_KEY_S])
	  	translation.x -= cameraSpeed;
  	if(KEYBUFFER[GLFW_KEY_A])
	  	translation.z += cameraSpeed;
  	if(KEYBUFFER[GLFW_KEY_D])
	  	translation.z -= cameraSpeed;
	direction += translation;
}

