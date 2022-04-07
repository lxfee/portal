#include "Camera.h"

Camera::Camera(ProjectMode projMode) : projMode(projMode) { 
	up = glm::vec3(0.0, 1.0, 0.0);  	// 初始UP
	eye = glm::vec3(0.0, 0.0, 3.0); 	// 初始位置
	dir = glm::vec3(0.0, 0.0, -1.0); 	// 初始up
};

Camera::~Camera() {
	if(pannel) delete pannel;
}

glm::mat4 Camera::getViewMatrix() {
	if(glm::length(glm::cross(up, dir)) < EPSILON) {
		if(glm::length(glm::cross(glm::vec3(1.0, 0.0, 0.0), dir)) > EPSILON) return glm::lookAt(eye, dir + eye, glm::vec3(1.0, 0.0, 0.0));
		if(glm::length(glm::cross(glm::vec3(0.0, 1.0, 0.0), dir)) > EPSILON) return glm::lookAt(eye, dir + eye, glm::vec3(0.0, 1.0, 0.0));
		if(glm::length(glm::cross(glm::vec3(0.0, 0.0, 1.0), dir)) > EPSILON) return glm::lookAt(eye, dir + eye, glm::vec3(0.0, 0.0, 1.0));
	} else {
		return glm::lookAt(eye, dir + eye, up);
	}
}

glm::mat4 Camera::getClippedProjectionMatrix(glm::mat4 viewMat, glm::mat4 projMat, glm::vec4 clipPlane) { //四维向量表示门平面的解析式，向量+点
	clipPlane = glm::transpose(glm::inverse(viewMat)) * clipPlane;
	glm::vec4 q;
	q.x = (glm::sign(clipPlane.x) + projMat[2][0]) / projMat[0][0];
	q.y = (glm::sign(clipPlane.y) + projMat[2][1]) / projMat[1][1];
	q.z = -1.0F;
	q.w = (1.0F + projMat[2][2]) / projMat[3][2];
	glm::vec4 c = clipPlane * (2.0f / glm::dot(clipPlane, q));
	projMat[0][2] = c.x;
	projMat[1][2] = c.y;
	projMat[2][2] = c.z + 1.0F;
	projMat[3][2] = c.w;
	return projMat;
}



glm::mat4 Camera::getProjectionMatrix() {
	switch(projMode) {
		case PERSPECTIVE: 
			if(!pannel) return glm::perspective(glm::radians(fov), aspect, near, far);
			else return getClippedProjectionMatrix(getViewMatrix(), glm::perspective(glm::radians(fov), aspect, near, far), *pannel);
		case ORTHO: 
			if(!pannel) return glm::ortho(-scale, scale, -scale, scale, this->near, this->far);
			else return getClippedProjectionMatrix(getViewMatrix(), glm::ortho(-scale, scale, -scale, scale, this->near, this->far), *pannel);
        default: assert(0);
    }
}

glm::vec3 Camera::doMovement() {
	extern unsigned char KEYBUFFER[1024];
	extern float frameTime;
	float cameraSpeed = 5.0f * (frameTime / 1000);
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
