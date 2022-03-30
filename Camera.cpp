#include "Camera.h"

Camera::Camera(ProjectMode projMode) : projMode(projMode) { 
	up = glm::vec3(0.0, 1.0, 0.0);  	// 初始UP
	eye = glm::vec3(0.0, 0.0, 3.0); 	// 初始位置
	dir = glm::vec3(0.0, 0.0, -1.0); 	// 初始up
};

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
	mouseMotion(0, 0);
	return glm::lookAt(eye, dir + eye, up);
}

glm::mat4 Camera::getProjectionMatrix() {
	switch(projMode) {
		case PERSPECTIVE: return glm::perspective(glm::radians(fov), aspect, near, far);
		case ORTHO: return glm::ortho(-scale, scale, -scale, scale, this->near, this->far);
        default: assert(0);
    }
}

glm::vec3 Camera::doMovement() {
	extern unsigned char KEYBUFFER[1024];
	extern float frameTime;
	float cameraSpeed = 8.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	dir = normalize(dir);
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
	float pitch, yaw;
	getPitchYaw(pitch, yaw);
	pitch += dpitch;
	yaw += dyaw;
	if(pitch > maxPitch) pitch = maxPitch;
	if(pitch < -maxPitch) pitch = -maxPitch;
	setPitchYaw(pitch, yaw);
}

void Camera::mouseWheel(int button, int dir, int x, int y) {
	if(fov >= 1.0f && fov <= 120.0f) fov -= dir;
	if(fov <= 1.0f) fov = 1.0f;
	if(fov >= 120.0f) fov = 120.0f;
}

void Camera::transCamera(Shader* shader) {
	shader->setMat4("view", getViewMatrix());
	shader->setMat4("projection", getProjectionMatrix());
	shader->setVec3("eyePos", eye);
}

void Camera::getPitchYaw(float &pitch, float &yaw) {
	pitch = glm::degrees(asin(dir.y));
	glm::vec3 right = glm::normalize(glm::cross(up, dir));
	glm::vec3 front = glm::normalize(glm::cross(right, up));
	yaw = glm::degrees(atan2(front.z, front.x));
	if(isnan(yaw)) yaw = 0;
}

void Camera::setPitchYaw(float pitch, float yaw) {
	dir.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	dir.y = sin(glm::radians(pitch));
	dir.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	dir = glm::normalize(dir);
}
