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

glm::vec3 Camera::doMovement(unsigned char* KEYBUFFER) {
	extern float frameTime;
	float cameraSpeed = 10.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	dir = normalize(dir);
	glm::vec3 right = glm::normalize(glm::cross(up, dir));
	glm::vec3 front = glm::normalize(glm::cross(right, up));

	if(KEYBUFFER[KEY_W]) translation += front * cameraSpeed;
  	if(KEYBUFFER[KEY_S]) translation -= front * cameraSpeed;
  	if(KEYBUFFER[KEY_A]) translation += right * cameraSpeed;
  	if(KEYBUFFER[KEY_D]) translation -= right * cameraSpeed;
	if(KEYBUFFER[' ']) translation += up * cameraSpeed;
	if(KEYBUFFER['q']) translation -= up * cameraSpeed;

	eye += translation;
	return translation;
}


void Camera::mouseMotion(float deltaX, float deltaY) {
	float sensitivity = 0.2f;
	float pitch = glm::degrees(asin(dir.y));
	glm::vec3 right = glm::normalize(glm::cross(up, dir));
	glm::vec3 front = glm::normalize(glm::cross(right, up));
	float yaw = glm::degrees(atan2(front.z, front.x));
	if(isnan(yaw)) yaw = 0;
	
	pitch += sensitivity * deltaY;
	yaw += sensitivity * deltaX;
	if(pitch > maxPitch) pitch = maxPitch;
	if(pitch < -maxPitch) pitch = -maxPitch;
	if(yaw > maxYaw) yaw = maxYaw;
	if(yaw < -maxYaw) yaw = -maxYaw;
	dir.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	dir.y = sin(glm::radians(pitch));
	dir.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	dir = glm::normalize(dir);
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