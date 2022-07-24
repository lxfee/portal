#include "Camera.h"
#define M_PI       3.14159265358979323846   // pi

Camera::Camera(ProjectMode projMode) : projMode(projMode) { 
	eye = glm::vec3(0.0, 0.0, 3.0); 	// 初始位置
	pitch = 0.0f;
	yaw = 0.0f;
};

Camera::~Camera() {}

glm::mat4 Camera::getRotation() {
	glm::mat4 rotat = glm::mat4(1.0f);
	rotat = glm::rotate(rotat, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	rotat = glm::rotate(rotat, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	return rotat;
}

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, -eye);
	return glm::inverse(getRotation()) * trans;
}



glm::mat4 Camera::getProjectionMatrix() {
	switch(projMode) {
		case PERSPECTIVE: 
			return glm::perspective(glm::radians(fov), aspect, near, far);
		case ORTHO: 
			return glm::ortho(-scale, scale, -scale, scale, this->near, this->far);
        default: 
			return glm::mat4(1.0f);
    }
}

glm::vec3 Camera::doMovement() {
	extern unsigned char KEYBUFFER[1024];
	extern float frameTime;
	float cameraSpeed = 5.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	glm::vec4 dir4 = getRotation() * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	glm::vec4 up4 = getRotation() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec3 dir(dir4.x, dir4.y, dir4.z);
	glm::vec3 up(up4.x, up4.y, up4.z);

	glm::vec3 right = glm::normalize(glm::cross(up, dir));
	glm::vec3 front = glm::normalize(glm::cross(right, up));

	if(KEYBUFFER['w']) translation += front * cameraSpeed;
  	if(KEYBUFFER['s']) translation -= front * cameraSpeed;
  	if(KEYBUFFER['a']) translation += right * cameraSpeed;
  	if(KEYBUFFER['d']) translation -= right * cameraSpeed;
	if(KEYBUFFER[' ']) translation += up * cameraSpeed;
	if(KEYBUFFER['q']) translation -= up * cameraSpeed;

	eye += translation;
	return translation;
}

void Camera::mouseMotion(float mouseDeltaX, float mouseDeltaY) {
	float sensitivity = 0.2f;
	float dpitch = sensitivity * mouseDeltaY;
	float dyaw = sensitivity * mouseDeltaX; 
	pitch += dpitch;
	yaw -= dyaw;
	if(pitch > 89.0) pitch = 89.0;
	if(pitch < -89.0) pitch = -89.0;
}

void Camera::mouseWheel(int button, int dir, int x, int y) {
	if(fov >= 1.0f && fov <= 120.0f) fov -= dir;
	if(fov <= 1.0f) fov = 1.0f;
	if(fov >= 120.0f) fov = 120.0f;
}

void Camera::transCamera(ShaderPtr shader) {
	shader->setMat4("view", getViewMatrix());
	shader->setMat4("projection", getProjectionMatrix());
	shader->setVec3("eyePos", eye);
}
