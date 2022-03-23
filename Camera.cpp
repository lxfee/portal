#include "Camera.h"

Camera::Camera() { 
	up = glm::vec3(0.0, 1.0, 0.0); // 初始UP
	eye = glm::vec3(0.0, 0.0, 3.0); // 初始位置
	yaw = 0;
	pitch = 0;
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	dir = glm::vec4(normalize(front), 0); // 初始朝向

	projMode = PERSPECTIVE;
};

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(eye, dir + eye, up);
}

glm::mat4 Camera::getProjectionMatrix() {
	switch(projMode) {
		case PERSPECTIVE: return glm::perspective(glm::radians(fov), aspect, near, far);
		case ORTHO: return glm::ortho(-scale, scale, -scale, scale, this->near, this->far);
        default: Print("未指明投影类型");
    }
}

void Camera::doMovement(unsigned char* KEYBUFFER) {
	extern float frameTime;
	float cameraSpeed = 10.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	glm::vec3 right = glm::normalize(glm::cross(up, dir));
	glm::vec3 updir = glm::normalize(glm::cross(dir, right));

	if(KEYBUFFER[GLFW_KEY_W]) 
	  	translation += dir * cameraSpeed;
  	if(KEYBUFFER[GLFW_KEY_S])
	  	translation -= dir * cameraSpeed;
  	if(KEYBUFFER[GLFW_KEY_A])
	  	translation += right * cameraSpeed;
  	if(KEYBUFFER[GLFW_KEY_D])
	  	translation -= right * cameraSpeed;
	if(KEYBUFFER[' ']) // 上
		translation += updir * cameraSpeed;
	if(KEYBUFFER['q']) // 下
		translation -= updir * cameraSpeed;
	eye += translation;
}



// 相机视角转动函数
void Camera::mouseMotion(float deltaX, float deltaY) {
	float sensitivity = 0.2f;

	yaw += sensitivity * deltaX;
	pitch += sensitivity * deltaY; 


	if(pitch > 89.0f) pitch =  89.0f;
	if(pitch < -89.0f) pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	dir = glm::vec4(normalize(front), 0);
}


// 相机焦距改变
void Camera::mouseWheel(int button, int dir, int x, int y) {
	if(fov >= 1.0f && fov <= 45.0f) fov -= dir;
	if(fov <= 1.0f) fov = 1.0f;
	if(fov >= 45.0f) fov = 45.0f;
}

void Camera::transCamera(Shader* shader) {
	shader->setMat4("vp", getProjectionMatrix() * getViewMatrix());
	shader->setVec3("eyePos", eye);
}